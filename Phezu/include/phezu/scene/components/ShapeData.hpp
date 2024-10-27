#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

#include <memory>

class mat3;

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        ShapeData(Entity* entity, const Vector2& pivot = Vector2::Zero(), const Vector2& size = Vector2::One());
    public:
        void SetPivot(const Vector2& pivot);
        void SetSize(const Vector2& size);
        void Set(const Vector2& pivot, const Vector2& size);
        const glm::mat3& GetObjectToLocal() { return m_ObjectToLocal; }
    private:
        void SetObjectToLocal();
    private:
        Vector2 m_Pivot;
        Vector2 m_Size;
        glm::mat3 m_ObjectToLocal;
    };
}
