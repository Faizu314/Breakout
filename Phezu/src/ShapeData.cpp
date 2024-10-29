#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"

namespace Phezu {

    ShapeData::ShapeData(Entity* entity, const Vector2& pivot, const Vector2& size) : DataComponent(entity), m_Pivot(pivot), m_Size(size) {
        SetShapeToObject();
    }
    
    void ShapeData::SetPivot(const Vector2& pivot) {
        memcpy(&m_Pivot, &pivot, sizeof(Vector2));
        SetShapeToObject();
    }
    
    void ShapeData::SetSize(const Vector2& size) {
        memcpy(&m_Size, &size, sizeof(Vector2));
        SetShapeToObject();
    }
    
    void ShapeData::Set(const Vector2& pivot, const Vector2& size) {
        memcpy(&m_Pivot, &pivot, sizeof(Vector2));
        memcpy(&m_Size, &size, sizeof(Vector2));
        SetShapeToObject();
    }
    
    Vector2 ShapeData::GetVertexPosition(VertexType vertexType) const {
        float Sx = m_Size.X();
        float Sy = m_Size.Y();
        
        switch (vertexType) {
            case VertexType::UpLeft:
                return Vector2(-Sx / 2.0, Sy / 2.0);
            case VertexType::UpRight:
                return Vector2(Sx / 2.0, Sy / 2.0);
            case VertexType::DownLeft:
                return Vector2(-Sx / 2.0, -Sy / 2.0);
            case VertexType::DownRight:
                return Vector2(Sx / 2.0, -Sy / 2.0);
            default:
                return Vector2();
        }
    }
    
    Vector2 ShapeData::GetVertexObjectPosition(VertexType vertexType) const {
        Vector2 shapePos = GetVertexPosition(vertexType);
        glm::vec3 shapePos3(shapePos.X(), shapePos.Y(), 1.0);
        glm::vec3 objectPos3 = m_ShapeToObject * shapePos3;
        return Vector2(objectPos3.x, objectPos3.y);
    }
    
    void ShapeData::SetShapeToObject() {
        float Sx = m_Entity->GetTransformData()->GetScale().X();
        float Sy = m_Entity->GetTransformData()->GetScale().Y();
        float Px = m_Pivot.X();
        float Py = m_Pivot.Y();
        m_ShapeToObject[0][0] = Sx;
        m_ShapeToObject[1][1] = Sy;
        m_ShapeToObject[2][0] = -Px * Sx;
        m_ShapeToObject[2][1] = -Py * Sy;
    }
}
