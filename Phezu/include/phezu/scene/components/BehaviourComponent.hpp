#pragma once

#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class BehaviourComponent : public DataComponent {
    public:
        BehaviourComponent(Entity* entity, uint8_t componentID) : DataComponent(entity) {}
        
        virtual void Start() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void OnDestroy() = 0;
    };
}
