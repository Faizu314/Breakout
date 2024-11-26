#pragma once

#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class BehaviourComponentPrefabBase;
    
    class BehaviourComponent : public DataComponent {
    public:
        BehaviourComponent(Entity* entity, uint8_t componentPrefabID) : DataComponent(entity), m_ComponentPrefabID(componentPrefabID) {}
    public:
        virtual void Start() {};
        virtual void Update(float deltaTime) {};
        virtual void OnDestroy() {};
    public:
        uint8_t GetComponentPrefabID() { return m_ComponentPrefabID; }
    private:
        uint8_t m_ComponentPrefabID;
    };
}
