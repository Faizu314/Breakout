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
        
        return overlapX && overlapY;
    }
    
    void Physics::ResolveDynamicToStaticCollision(std::shared_ptr<Entity> dynamicEntity, std::shared_ptr<Entity> staticEntity, CollisionData& cd) {
        glm::mat2 reflectionMat(1);
        
        float xTranslate = 0, yTranslate = 0;
        float downPenY = glm::max(0.0f, cd.A.MaxY - cd.B.MinY);
        float upPenY = glm::max(0.0f, cd.B.MaxY - cd.A.MinY);
        float leftPenX = glm::max(0.0f, cd.A.MaxX - cd.B.MinX);
        float rightPenX = glm::max(0.0f, cd.B.MaxX - cd.A.MinX);
        
        if (rightPenX <= glm::min(downPenY, upPenY) && rightPenX <= leftPenX) {
            xTranslate = cd.B.MaxX - cd.A.MinX + EPSILON;
            reflectionMat[0].x = -1;
        }
        else if (leftPenX <= glm::min(downPenY, upPenY) && rightPenX >= leftPenX) {
            xTranslate = cd.B.MinX - cd.A.MaxX - EPSILON;
            reflectionMat[0].x = -1;
        }
        if (upPenY <= glm::min(leftPenX, rightPenX) && upPenY <= downPenY) {
            yTranslate = cd.B.MaxY - cd.A.MinY + EPSILON;
            reflectionMat[1].y = -1;
        }
        else if (downPenY <= glm::min(leftPenX, rightPenX) && upPenY >= downPenY) {
            yTranslate = cd.B.MinY - cd.A.MaxY - EPSILON;
            reflectionMat[1].y = -1;
        }
        
        auto trans = dynamicEntity->GetTransformData();
        auto phys = dynamicEntity->GetPhysicsData().lock();
        
        glm::vec reflectedVel = reflectionMat * phys->Velocity;
        phys->Velocity = Vector2(reflectedVel.x, reflectedVel.y);
        
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
