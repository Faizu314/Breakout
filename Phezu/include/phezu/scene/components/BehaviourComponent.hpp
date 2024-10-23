#pragma once

#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class BehaviourComponent : public DataComponent {
    public:
        BehaviourComponent(Entity* entity, uint8_t componentID) : DataComponent(entity) {}
        
        virtual void Start();
        virtual void Update();
        virtual void OnDestroy();
    };
}
