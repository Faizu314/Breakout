#pragma once

#include "maths/Math.hpp"

namespace Phezu {
    
    float RandomValue();
    float RandomValue(float inclusive, float exclusive);
    
    class Random {
    public:
        static Vector2 Direction();
        static Vector2 Direction(float minAngle, float maxAngle, AngleUnit units = AngleUnit::Degrees);
    };
}
