#include "maths/Math.hpp"

namespace Phezu {
    
    const Vector2 Vector2::One = Vector2(1, 1);
    const Vector2 Vector2::Zero = Vector2();
    const Vector2 Vector2::Right = Vector2(1, 0);
    const Vector2 Vector2::Left = Vector2(-1, 0);
    const Vector2 Vector2::Up = Vector2(0, 1);
    const Vector2 Vector2::Down = Vector2(0, -1);
    
    void Vector2::Normalize() {
        if (glm::length(m_Vector) == 0)
            return;
        m_Vector = glm::normalize(m_Vector);
    }
    
    float Vector2::SqrDistance(const Vector2 &a, const Vector2 &b) {
        float dx = a.X() - b.X();
        float dy = a.Y() - b.Y();
        
        return (dx * dx) + (dy * dy);
    }
    
    int RoundToPixel(float value) {
        int x = glm::round(glm::abs(value));
        return glm::round(value + x) - x;
    }
    
    void RoundToPixel(Vector2& vec) {
        vec.Set(RoundToPixel(vec.X()), RoundToPixel(vec.Y()));
        return vec;
    }
}
