#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class PhysicsData;
    
    class Physics {
    public:
        Physics(Engine* engine);
    public:
        void PhysicsUpdate(const std::vector<std::weak_ptr<Entity>>& physicsEntities, size_t staticCount, size_t dynamicCount);
    private:
        void ResolveDynamicToStaticCollisions(std::shared_ptr<Entity> dynamicEntity, const std::vector<std::weak_ptr<Entity>>& physicsEntities, size_t staticCount);
        void ResolveDynamicToStaticCollision(std::shared_ptr<Entity> dynamicEntity, std::shared_ptr<Entity> staticEntity);
        bool IsColliding(std::shared_ptr<Entity> entityA, std::shared_ptr<Entity> entityB);
    private:
        void CleanCollidingEntities();
        bool WereColliding(std::shared_ptr<PhysicsData> a, std::shared_ptr<PhysicsData> b);
        void OnColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
        void OnNotColliding(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
        void RemoveCollisionPair(std::shared_ptr<PhysicsData> a, std::shared_ptr<PhysicsData> b);
    private:
        Engine* m_Engine;
        std::vector<std::pair<std::weak_ptr<PhysicsData>, std::weak_ptr<PhysicsData>>> m_CollidingEntities;
    };
}
