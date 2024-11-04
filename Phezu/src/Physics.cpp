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
    void Physics::PhysicsUpdate(const std::vector<std::weak_ptr<Entity>>& physicsEntities, size_t staticCount, size_t dynamicCount) {
        CleanCollidingEntities();
        
        for (size_t i = staticCount; i < staticCount + dynamicCount; i++) {
            auto dynamicEntity = physicsEntities[i].lock();
                
            ResolveDynamicToStaticCollisions(dynamicEntity, physicsEntities, staticCount);
        }
    }
    
    void Physics::ResolveDynamicToStaticCollisions(std::shared_ptr<Entity> dynamicEntity, const std::vector<std::weak_ptr<Entity>>& physicsEntities, size_t staticCount) {
        for (size_t i = 0; i < staticCount; i++) {
            auto staticEntity = physicsEntities[i].lock();

            if (!IsColliding(dynamicEntity, staticEntity)) {
                OnNotColliding(dynamicEntity, staticEntity);
                continue;
            }
            
            OnColliding(dynamicEntity, staticEntity);
            ResolveDynamicToStaticCollision(dynamicEntity, staticEntity);
        }
    }
    
    bool Physics::IsColliding(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB) {
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
        
        int aMaxX = glm::max(ulA.X(), drA.X());
        int aMinX = glm::min(ulA.X(), drA.X());
        int aMaxY = glm::max(ulA.Y(), drA.Y());
        int aMinY = glm::min(ulA.Y(), drA.Y());
        
        int bMaxX = glm::max(ulB.X(), drB.X());
        int bMinX = glm::min(ulB.X(), drB.X());
        int bMaxY = glm::max(ulB.Y(), drB.Y());
        int bMinY = glm::min(ulB.Y(), drB.Y());
        
        bool overlapX = aMinX <= bMaxX && aMaxX >= bMinX;
        bool overlapY = aMinY <= bMaxY && aMaxY >= bMinY;

        return overlapX && overlapY;
    }
    
    void Physics::ResolveDynamicToStaticCollision(std::shared_ptr<Entity> dynamicEntity, std::shared_ptr<Entity> staticEntity) {
        auto physicsData = dynamicEntity->GetPhysicsData();
                
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
