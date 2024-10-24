#pragma once

#include <cstdint>
#include <memory>

namespace Phezu {
    
    class Scene;
    class Entity;
    class BehaviourComponent;
    
    class BehaviourComponentPrefab {
    public:
        BehaviourComponentPrefab(uint64_t prefabID, std::unique_ptr<size_t[]> pathToPrefabEntity, uint8_t componentID);
        uint64_t GetEntityPrefabID();
        uint8_t GetComponentID();
        virtual std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const = 0;
        std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::weak_ptr<Scene> scene, uint64_t instanceID);
        std::weak_ptr<Entity> GetRuntimeEntity(std::weak_ptr<Scene> scene, uint64_t instanceID);
        virtual void LinkRuntimeEntityAndComponentRefs(std::weak_ptr<Scene> scene, uint64_t instanceID);
    protected:
        const uint64_t m_PrefabID;
        const uint8_t m_ComponentID;
        std::unique_ptr<size_t[]> m_PathToPrefabEntity;
    };
}
