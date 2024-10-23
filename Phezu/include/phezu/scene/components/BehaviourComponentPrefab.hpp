#pragma once

#include <cstdint>
#include <memory>

namespace Phezu {
    
    class Entity;
    class BehaviourComponent;
    
    class BehaviourComponentPrefab {
    public:
        BehaviourComponentPrefab(uint64_t entityPrefab, uint8_t componentID);
        uint64_t GetEntityPrefabID();
        uint8_t GetComponentID();
        virtual std::weak_ptr<BehaviourComponent> CreateComponent(Entity* entity) const = 0;
        virtual void LinkRuntimeEntityAndComponentRefs(uint64_t instanceID) = 0;
    protected:
        const uint64_t m_EntityPrefab;
        const uint8_t m_ComponentID;
    };
}
