#pragma once

#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class BehaviourComponentPrefabBase;
    
    class BehaviourComponent : public DataComponent {
    public:
        BehaviourComponent(Entity* entity, uint8_t componentPrefabID) : DataComponent(entity), m_ComponentPrefabID(componentPrefabID) {}
    public:
        virtual void Start() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void OnDestroy() = 0;
    public:
        uint8_t GetComponentPrefabID() { return m_ComponentPrefabID; }
    private:
        uint8_t m_ComponentPrefabID;
    };
}
