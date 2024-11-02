#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"

namespace Phezu {

    ShapeData::ShapeData(Entity* entity, const Vector2& pivot, const Vector2& size) : DataComponent(entity), m_Pivot(pivot), m_Size(size) {}
    
    void ShapeData::SetPivot(const Vector2& pivot) {
        memcpy(&m_Pivot, &pivot, sizeof(Vector2));
    }
    
    void ShapeData::SetSize(const Vector2& size) {
        memcpy(&m_Size, &size, sizeof(Vector2));
    }
    
    void ShapeData::Set(const Vector2& pivot, const Vector2& size) {
        SetPivot(pivot);
        SetSize(size);
    }
    
    Vector2 ShapeData::GetVertexPosition(VertexType vertexType) const {
        float Sx = m_Size.X();
        float Sy = m_Size.Y();
        
        switch (vertexType) {
            case VertexType::UpLeft:
                return Vector2(-Sx / 2.0, Sy / 2.0) - m_Pivot;
            case VertexType::UpRight:
                return Vector2(Sx / 2.0, Sy / 2.0) - m_Pivot;
            case VertexType::DownLeft:
                return Vector2(-Sx / 2.0, -Sy / 2.0) - m_Pivot;
            case VertexType::DownRight:
                return Vector2(Sx / 2.0, -Sy / 2.0) - m_Pivot;
            default:
                return Vector2();
        }
    }
}
