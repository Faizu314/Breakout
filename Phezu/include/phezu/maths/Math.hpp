#pragma once

#include "glm/glm.hpp"

namespace Phezu {
    
    class Vector2 {
    public:
        Vector2() : m_Vector(0, 0) { }
        Vector2(float x, float y) : m_Vector(x, y) {}
        Vector2(const Vector2& vec) : m_Vector(vec.X(), vec.Y()) {}
    public:
        float X() const { return m_Vector.x; }
        float Y() const { return m_Vector.y; }
        void SetX(float x) { m_Vector.x = x; }
        void SetY(float y) { m_Vector.y = y; }
        void Set(float x, float y) { SetX(x); SetY(y); }
    public:
        float Dot(const Vector2& other) { return glm::dot(other.m_Vector, m_Vector); }
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
        Vector2& operator=(const Vector2& other) { Set(other.X(), other.Y()); return *this; }
        Vector2 operator*(float value) { Set(m_Vector.x * value, m_Vector.y * value); }
        Vector2 operator/(float value) { Set(m_Vector.x / value, m_Vector.y / value); }
        operator glm::vec2&() { return m_Vector; }
        operator const glm::vec2&() const { return m_Vector; }
    private:
        Vector2(const glm::vec2& other) : m_Vector(other.x, other.y) {}
        glm::vec2 m_Vector;
    };
}
