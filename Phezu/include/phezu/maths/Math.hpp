#pragma once

#include "glm/glm.hpp"

namespace Phezu {
    
    enum class AngleUnit {
        Degrees, Radians
    };
    
    class Vector2 {
    public:
        Vector2() : m_Vector(0, 0) { }
        Vector2(float x, float y) : m_Vector(x, y) {}
        Vector2(const Vector2& vec) : m_Vector(vec.X(), vec.Y()) {}
    public:
        float X() const { return m_Vector.x; }
        float Y() const { return m_Vector.y; }
        Vector2 SetX(float x) { m_Vector.x = x; return *this; }
        Vector2 SetY(float y) { m_Vector.y = y; return *this; }
        Vector2 Set(float x, float y) { SetX(x); return SetY(y); }
    public:
        void Normalize();
        Vector2 Normalized() const { Vector2 a(*this); a.Normalize(); return a; }
        float Length() const { return Distance(*this, Zero); }
        float SqrLength() const { return SqrDistance(*this, Zero); }
    public:
        float Dot(const Vector2& other) const { return Dot(*this, other); }
        float Cross(const Vector2& other) const { return Cross(*this, other); }
        float Distance(const Vector2& other) const { return Distance(*this, other); }
        float SqrDistance(const Vector2& other) const { return SqrDistance(*this, other); }
    public:
        static float Dot(const Vector2& a, const Vector2& b) { return glm::dot(a.m_Vector, b.m_Vector); }
        static float Cross(const Vector2& a, const Vector2& b) { return (a.X() * b.Y()) - (a.Y() * b.X()); }
        static float Distance(const Vector2& a, const Vector2& b) { return glm::distance(a.m_Vector, b.m_Vector); }
        static float SqrDistance(const Vector2& a, const Vector2& b);
    public:
        static const Vector2 One;
        static const Vector2 Zero;
        static const Vector2 Right;
        static const Vector2 Left;
        static const Vector2 Up;
        static const Vector2 Down;
    public:
        Vector2 operator+(const Vector2& other) const { return Vector2(m_Vector.x + other.m_Vector.x, m_Vector.y + other.m_Vector.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(m_Vector.x - other.m_Vector.x, m_Vector.y - other.m_Vector.y); }
        Vector2& operator=(const Vector2& other) { Set(other.X(), other.Y()); return *this; }
        Vector2 operator*(float value) const { return Vector2(m_Vector * value); }
        Vector2 operator/(float value) const { return Vector2(m_Vector / value); }
        operator glm::vec2&() { return m_Vector; }
        operator const glm::vec2&() const { return m_Vector; }
    private:
        Vector2(const glm::vec2& other) : m_Vector(other.x, other.y) {}
        glm::vec2 m_Vector;
    };
    
    inline float Clamp01(float value) {
        return glm::clamp<float>(value, 0, 1);
    }
    
    inline float Clamp(float value, float min, float max) {
        return glm::clamp(value, min, max);
    }
    
    inline float Lerp(float start, float end, float t) {
        t = Clamp01(t);
        return start + (end - start) * t;
    }
    
    inline int Sign(float value) {
        return glm::sign(value);
    }
    
    inline float Sin(float angle, AngleUnit units = AngleUnit::Degrees) {
        float appAngle = units == AngleUnit::Radians ? angle : glm::radians(angle);
        return glm::sin(appAngle);
    }
    
    inline float Cos(float angle, AngleUnit units = AngleUnit::Degrees) {
        float appAngle = units == AngleUnit::Radians ? angle : glm::radians(angle);
        return glm::cos(appAngle);
    }
    
    int RoundToPixel(float value);
    void RoundToPixel(Vector2& vec);
}
