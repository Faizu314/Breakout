#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class TransformData : public DataComponent {
    public:
        Vector2 Position;
        Vector2 Scale;
    };
}
