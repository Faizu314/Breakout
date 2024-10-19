#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class TransformData : public DataComponent {
    public:
        TransformData(Entity* entity) : DataComponent(entity) {}
        Vector2 Position;
        Vector2 Scale;
    };
}
