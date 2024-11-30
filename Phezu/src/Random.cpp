#include "Random.hpp"

#include <random>

namespace Phezu {
    
    static std::random_device _Seed;
    static std::mt19937 _Generator(_Seed());
    static std::uniform_real_distribution<float> _Distribution(0.0, 1.0);
    
    float RandomValue() {
        return _Distribution(_Generator);
    }
    
    float RandomValue(float inclusive, float exclusive) {
        float lowerBound = glm::min(inclusive, exclusive);
        float upperBound = glm::max(inclusive, exclusive);
        float range = upperBound - lowerBound;
        
        return (RandomValue() * range) + lowerBound;
    }
    
    Vector2 Random::Direction() {
        float angle = RandomValue() * 360.0f;
        return Vector2(Cos(angle), Sin(angle));
    }
    
    Vector2 Random::Direction(float minAngle, float maxAngle, AngleUnit units) {
        float angle = RandomValue(minAngle, maxAngle);
        return Vector2(Cos(angle, units), Sin(angle, units));
    }
}
