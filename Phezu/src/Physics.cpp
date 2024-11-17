#include "Phezu.hpp"

#include <algorithm>

namespace Phezu {
    
    const float Physics::EPSILON = 0.000000;
    
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
    
    Physics::EntityRect Physics::GetWorldRectFromTransformAndShapeData(TransformData* transData, ShapeData* shapeData) {
        Vector2 ul = transData->LocalToWorldPoint(shapeData->GetVertexPosition(ShapeData::VertexType::UpLeft));
        Vector2 dr = transData->LocalToWorldPoint(shapeData->GetVertexPosition(ShapeData::VertexType::DownRight));
        
        Physics::EntityRect rect;
        
        rect.MaxX = glm::max(ul.X(), dr.X());
        rect.MinX = glm::min(ul.X(), dr.X());
        rect.MaxY = glm::max(ul.Y(), dr.Y());
        rect.MinY = glm::min(ul.Y(), dr.Y());
        
        return rect;
    }
    
    bool Physics::IsColliding(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB, CollisionData& cd) {        
        auto transA = entityA->GetTransformData();
        auto shapeA = entityA->GetShapeData();
        
        auto transB = entityB->GetTransformData();
        auto shapeB = entityB->GetShapeData();

        cd.A = GetWorldRectFromTransformAndShapeData(transA, shapeA);
        cd.B = GetWorldRectFromTransformAndShapeData(transB, shapeB);
        
        bool overlapX = cd.A.MinX < cd.B.MaxX && cd.A.MaxX > cd.B.MinX;
        bool overlapY = cd.A.MinY < cd.B.MaxY && cd.A.MaxY > cd.B.MinY;
        
        if (overlapX && overlapY) {
            auto physicsDataA = entityA->GetPhysicsData().lock();
            auto physicsDataB = entityB->GetPhysicsData().lock();
            
            Vector2 originalPosA = transA->GetLocalPosition();
            Vector2 originalPosB = transB->GetLocalPosition();
            
            transA->SetLocalPosition(physicsDataA->m_SimulationData.PrevPosition);
            transB->SetLocalPosition(physicsDataB->m_SimulationData.PrevPosition);
            transA->RecalculateLocalToWorld();
            transB->RecalculateLocalToWorld();
            
            EntityRect aPrev = GetWorldRectFromTransformAndShapeData(transA, shapeA);
            EntityRect bPrev = GetWorldRectFromTransformAndShapeData(transB, shapeB);
            
            transA->SetLocalPosition(originalPosA);
            transB->SetLocalPosition(originalPosB);
            transA->RecalculateLocalToWorld();
            transB->RecalculateLocalToWorld();
            
            bool prevOverlapX = aPrev.MinX < bPrev.MaxX && aPrev.MaxX > bPrev.MinX;
            bool prevOverlapY = aPrev.MinY < bPrev.MaxY && aPrev.MaxY > bPrev.MinY;
            
            cd.PenetrationX = !prevOverlapX;
            cd.PenetrationY = !prevOverlapY;
        }
        
        return overlapX && overlapY;
    }
    
    void Physics::ResolveDynamicToStaticCollision(std::shared_ptr<Entity> dynamicEntity, std::shared_ptr<Entity> staticEntity, CollisionData& cd) {
        Vector2 velA = (dynamicEntity->GetTransformData()->GetLocalPosition() - dynamicEntity->GetPhysicsData().lock()->m_SimulationData.PrevPosition) / m_DeltaTime;
        Vector2 velB = (staticEntity->GetTransformData()->GetLocalPosition() - staticEntity->GetPhysicsData().lock()->m_SimulationData.PrevPosition) / m_DeltaTime;
        
        Vector2 relativeVel = velA - velB;
        
        float xTranslate = 0, yTranslate = 0;
        
        float downPenY = glm::max(0.0f, cd.A.MaxY - cd.B.MinY);
        float upPenY = glm::max(0.0f, cd.B.MaxY - cd.A.MinY);
        float leftPenX = glm::max(0.0f, cd.A.MaxX - cd.B.MinX);
        float rightPenX = glm::max(0.0f, cd.B.MaxX - cd.A.MinX);
        
        if (cd.PenetrationX) {
            if (relativeVel.X() < 0 && rightPenX < leftPenX) {
                xTranslate = cd.B.MaxX - cd.A.MinX + EPSILON;
            }
            else if (leftPenX < rightPenX) {
                xTranslate = cd.B.MinX - cd.A.MaxX - EPSILON;
            }
        }
        if (cd.PenetrationY) {
            if (relativeVel.Y() < 0 && upPenY < downPenY) {
                yTranslate = cd.B.MaxY - cd.A.MinY + EPSILON;
            }
            else if (downPenY < upPenY) {
                yTranslate = cd.B.MinY - cd.A.MaxY - EPSILON;
            }
        }
        if (!cd.PenetrationX && !cd.PenetrationY) {
            //prev collision not resolved properly due to floating point precision
            
            if (rightPenX <= leftPenX && rightPenX <= glm::min(downPenY, upPenY)) {
                xTranslate = cd.B.MaxX - cd.A.MinX + EPSILON;
            }
            else if (rightPenX >= leftPenX && leftPenX <= glm::min(downPenY, upPenY)) {
                xTranslate = cd.B.MinX - cd.A.MaxX - EPSILON;
            }
            if (upPenY <= downPenY && upPenY <= glm::min(leftPenX, rightPenX)) {
                yTranslate = cd.B.MaxY - cd.A.MinY + EPSILON;
            }
            else if (upPenY >= downPenY && downPenY <= glm::min(leftPenX, rightPenX)) {
                yTranslate = cd.B.MinY - cd.A.MaxY - EPSILON;
            }
        }
        
        auto trans = dynamicEntity->GetTransformData();
        trans->SetLocalPosition(trans->GetLocalPosition() + Vector2(xTranslate, yTranslate));
        trans->RecalculateLocalToWorld();
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
