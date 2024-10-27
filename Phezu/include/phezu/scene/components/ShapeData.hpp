#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

#include <memory>

class mat3;

namespace Phezu {
    
    class ShapeData : public DataComponent {
    public:
        enum VertexType {
            None = 0,
            UpLeft = 1,
            UpRight = 2,
            DownLeft = 3,
            DownRight = 4,
        };
    public:
        ShapeData(Entity* entity, const Vector2& pivot = Vector2::Zero(), const Vector2& size = Vector2::One());
    public:
        void SetPivot(const Vector2& pivot);
        void SetSize(const Vector2& size);
        void Set(const Vector2& pivot, const Vector2& size);
        const glm::mat3& GetShapeToObject() const { return m_ShapeToObject; }
        Vector2 GetVertexPosition(VertexType vertexType) const;
        Vector2 GetVertexObjectPosition(VertexType vertexType) const;
    private:
        void SetShapeToObject();
    private:
        Vector2 m_Pivot;
        Vector2 m_Size;
        glm::mat3 m_ShapeToObject;
    };
}
