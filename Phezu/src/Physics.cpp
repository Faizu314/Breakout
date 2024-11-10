#include "Phezu.hpp"

#include <algorithm>

namespace Phezu {
    
    Physics::Physics(Engine* engine) : m_Engine(engine) {}
    
    void Physics::CleanCollidingEntities() {
        for (auto it = m_CollidingEntities.begin(); it != m_CollidingEntities.end();) {
            auto e1 = (*it).first;
            auto e2 = (*it).second;
            
            if (e1.expired() || e2.expired()) {
                if (e1.expired() && !e2.expired())
                    e2.lock()->OnCollisionExit(Collision(e1));
                else if (e2.expired() && !e1.expired())
                    e1.lock()->OnCollisionExit(Collision(e2));
                it = m_CollidingEntities.erase(it);
            }
            else
                it++;
        }
    }
    
    // assuming all physics entities are valid as scene cannot return entities that were destroyed.
    // assuming all these physics entities have valid physicsData components as scene checked for that.
    void Physics::PhysicsUpdate(const std::vector<std::weak_ptr<Entity>>& physicsEntities, size_t staticCount, size_t dynamicCount, float deltaTime) {
        m_DeltaTime = deltaTime;
        
        CleanCollidingEntities();
        
        //Apply velocities of everything
        
        for (size_t i = staticCount; i < staticCount + dynamicCount; i++) {
            auto dynamicEntity = physicsEntities[i].lock();
                
            ResolveDynamicToStaticCollisions(dynamicEntity, physicsEntities, staticCount);
        }
        
        for (size_t i = 0; i < staticCount + dynamicCount; i++) {
            auto entityL = physicsEntities[i].lock();
            
            entityL->GetPhysicsData().lock()->m_SimulationData.PrevPosition = entityL->GetTransformData()->GetLocalPosition();
        }
    }
    
    void Physics::ResolveDynamicToStaticCollisions(std::shared_ptr<Entity> dynamicEntity, const std::vector<std::weak_ptr<Entity>>& physicsEntities, size_t staticCount) {
        
        CollisionData collisionData;
        for (size_t i = 0; i < staticCount; i++) {
            auto staticEntity = physicsEntities[i].lock();

            if (!IsColliding(dynamicEntity, staticEntity, collisionData)) {
                OnNotColliding(dynamicEntity, staticEntity);
                continue;
            }
            
            OnColliding(dynamicEntity, staticEntity);
            ResolveDynamicToStaticCollision(dynamicEntity, staticEntity, collisionData);
        }
    }
    
    bool Physics::IsColliding(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB, CollisionData& cd) {
        auto physicsDataA = entityA->GetPhysicsData().lock();
        auto physicsDataB = entityB->GetPhysicsData().lock();
        
        auto transA = entityA->GetTransformData();
        auto shapeA = entityA->GetShapeData();
        Vector2 ulA = transA->LocalToWorldPoint(shapeA->GetVertexPosition(ShapeData::VertexType::UpLeft));
        Vector2 drA = transA->LocalToWorldPoint(shapeA->GetVertexPosition(ShapeData::VertexType::DownRight));
        
        auto transB = entityB->GetTransformData();
        auto shapeB = entityB->GetShapeData();
        Vector2 ulB = transB->LocalToWorldPoint(shapeB->GetVertexPosition(ShapeData::VertexType::UpLeft));
        Vector2 drB = transB->LocalToWorldPoint(shapeB->GetVertexPosition(ShapeData::VertexType::DownRight));
        
        cd.A.MaxX = RoundToPixel(glm::max(ulA.X(), drA.X()));
        cd.A.MinX = RoundToPixel(glm::min(ulA.X(), drA.X()));
        cd.A.MaxY = RoundToPixel(glm::max(ulA.Y(), drA.Y()));
        cd.A.MinY = RoundToPixel(glm::min(ulA.Y(), drA.Y()));
        
        cd.PixelCorrectionA = Vector2(cd.A.MaxX - glm::max(ulA.X(), drA.X()), cd.A.MaxY - glm::max(ulA.Y(), drA.Y()));
        
        cd.B.MaxX = RoundToPixel(glm::max(ulB.X(), drB.X()));
        cd.B.MinX = RoundToPixel(glm::min(ulB.X(), drB.X()));
        cd.B.MaxY = RoundToPixel(glm::max(ulB.Y(), drB.Y()));
        cd.B.MinY = RoundToPixel(glm::min(ulB.Y(), drB.Y()));
        
        cd.PixelCorrectionB = Vector2(cd.B.MaxX - glm::max(ulB.X(), drB.X()), cd.B.MaxY - glm::max(ulB.Y(), drB.Y()));
        
        bool overlapX = cd.A.MinX < cd.B.MaxX && cd.A.MaxX > cd.B.MinX;
        bool overlapY = cd.A.MinY < cd.B.MaxY && cd.A.MaxY > cd.B.MinY;

        return overlapX && overlapY;
    }
    
    void Physics::ResolveDynamicToStaticCollision(std::shared_ptr<Entity> dynamicEntity, std::shared_ptr<Entity> staticEntity, CollisionData& collisionData) {
        float aMidX = (collisionData.A.MinX + collisionData.A.MaxX) / 2.0;
        float bMidX = (collisionData.B.MinX + collisionData.B.MaxX) / 2.0;
        int deltaX = aMidX - bMidX;
        
        float aMidY = (collisionData.A.MinY + collisionData.A.MaxY) / 2.0;
        float bMidY = (collisionData.B.MinY + collisionData.B.MaxY) / 2.0;
        int deltaY = aMidY - bMidY;
        
        Vector2 aVel = (dynamicEntity->GetTransformData()->GetLocalPosition() - dynamicEntity->GetPhysicsData().lock()->m_SimulationData.PrevPosition) / m_DeltaTime;
        Vector2 bVel = (staticEntity->GetTransformData()->GetLocalPosition() - staticEntity->GetPhysicsData().lock()->m_SimulationData.PrevPosition) / m_DeltaTime;
        
        Vector2 relativeVel = aVel - bVel;
        
        int xTranslate = 0, yTranslate = 0;
        
        if (glm::abs(relativeVel.X()) >= glm::abs(relativeVel.Y())) {
            if (relativeVel.X() < 0) {
                xTranslate = collisionData.B.MaxX - collisionData.A.MinX;
            }
            else {
                xTranslate = collisionData.B.MinX - collisionData.A.MaxX;
            }
        }
        if (glm::abs(relativeVel.X()) <= glm::abs(relativeVel.Y())) {
            if (relativeVel.Y() < 0) {
                yTranslate = collisionData.B.MaxY - collisionData.A.MinY;
            }
            else {
                yTranslate = collisionData.B.MinY - collisionData.A.MaxY;
            }
        }
        
        auto trans = dynamicEntity->GetTransformData();
        trans->SetLocalPosition(trans->GetLocalPosition() + Vector2(xTranslate, yTranslate) + collisionData.PixelCorrectionA);
        auto resolvedPos = trans->GetLocalPosition();
        
        printf("%llu | SMaxX: %i, DMinX: %i, Tx: %i, Rx: %f\n", m_Engine->GetFrameCount(), collisionData.B.MaxX, collisionData.A.MinX, xTranslate, resolvedPos.X());
        
    }
    
    void Physics::OnColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
        auto ap = a->GetPhysicsData().lock();
        auto bp = b->GetPhysicsData().lock();
        
        Collision cA(bp);
        Collision cB(ap);
        
        if (WereColliding(ap, bp)) {
            ap->OnCollisionStay(cA);
            bp->OnCollisionStay(cB);
        }
        else {
            ap->OnCollisionEnter(cA);
            bp->OnCollisionEnter(cB);
            m_CollidingEntities.emplace_back(ap, bp);
        }
    }
    
    void Physics::OnNotColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
        auto ap = a->GetPhysicsData().lock();
        auto bp = b->GetPhysicsData().lock();
        
        Collision cA(bp);
        Collision cB(ap);
        
        if (WereColliding(ap, bp)) {
            ap->OnCollisionExit(cA);
            bp->OnCollisionExit(cB);
            RemoveCollisionPair(ap, bp);
        }
    }
    
    bool Physics::WereColliding(std::shared_ptr<PhysicsData> a, std::shared_ptr<PhysicsData> b) {
        for (auto& pair : m_CollidingEntities) {
            auto sharedA = pair.first.lock();
            auto sharedB = pair.second.lock();
            auto sharedC = a;
            auto sharedD = b;
            
            return (sharedA == sharedC && sharedB == sharedD) || (sharedA == sharedD && sharedB == sharedC);
        }
        
        return false;
    }
    
    void Physics::RemoveCollisionPair(std::shared_ptr<PhysicsData> a, std::shared_ptr<PhysicsData> b) {
        for (auto it = m_CollidingEntities.begin(); it != m_CollidingEntities.end(); it++) {
            auto pair = *it;
            auto sharedA = pair.first.lock();
            auto sharedB = pair.second.lock();
            auto sharedC = a;
            auto sharedD = b;
            
            if ((sharedA == sharedC && sharedB == sharedD) || (sharedA == sharedD && sharedB == sharedC)) {
                m_CollidingEntities.erase(it);
                return;
            }
        }
    }
}
