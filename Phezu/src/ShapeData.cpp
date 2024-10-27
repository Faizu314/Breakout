#include "scene/components/ShapeData.hpp"

namespace Phezu {

    ShapeData::ShapeData(Entity* entity, const Vector2& pivot, const Vector2& size) : DataComponent(entity), m_Pivot(pivot), m_Size(size) {
        SetObjectToLocal();
    }
    
    void ShapeData::SetPivot(const Vector2& pivot) {
        memcpy(&m_Pivot, &pivot, sizeof(Vector2));
        SetObjectToLocal();
    }
    
    void ShapeData::SetSize(const Vector2& size) {
        memcpy(&m_Size, &size, sizeof(Vector2));
        SetObjectToLocal();
    }
    
    void ShapeData::Set(const Vector2& pivot, const Vector2& size) {
        memcpy(&m_Pivot, &pivot, sizeof(Vector2));
        memcpy(&m_Size, &size, sizeof(Vector2));
        SetObjectToLocal();
    }
    
    void ShapeData::SetObjectToLocal() {
        m_ObjectToLocal[0][0] = m_Size.X();
        m_ObjectToLocal[1][1] = m_Size.Y();
        m_ObjectToLocal[2][0] = -m_Pivot.X() * m_Size.X();
        m_ObjectToLocal[2][1] = -m_Pivot.Y() * m_Size.Y();
    }
}
