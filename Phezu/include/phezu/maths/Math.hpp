#pragma once

#include "glm/glm.hpp"
#include "SDL2/SDL.h"

namespace Phezu {
    
    class Vector2 {
    public:
        Vector2() : m_Vector(0, 0) { }
        Vector2(float x, float y) : m_Vector(x, y) { }
    public:
        static Vector2 One() { return Vector2(1, 1); }
        static Vector2 Zero() { return Vector2(); }
        static Vector2 Right() { return Vector2(1, 0); }
        static Vector2 Left() { return Vector2(-1, 0); }
        static Vector2 Up() { return Vector2(0, 1); }
        static Vector2 Down() { return Vector2(0, -1); }
    public:
        Vector2 operator+(const Vector2& other) const { return Vector2(m_Vector.x + other.m_Vector.x, m_Vector.y + other.m_Vector.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(m_Vector.x - other.m_Vector.x, m_Vector.y - other.m_Vector.y); }
        operator glm::vec2&() { return m_Vector; }
        operator const glm::vec2&() const { return m_Vector; }
    private:
        glm::vec2 m_Vector;
    };

    typedef struct {
        float x, y;
        float w, h;
    } Rect;
}
