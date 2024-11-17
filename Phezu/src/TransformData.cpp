#include "scene/Entity.hpp"
#include "scene/components/TransformData.hpp"
#include "glm/glm.hpp"

namespace Phezu {

    TransformData::TransformData(Entity* entity) : DataComponent(entity) {}
    
    void TransformData::SetLocalPosition(const Vector2& position) {
        m_LocalPosition = position;
        m_IsDirty = true;
    }
    
    Vector2 TransformData::GetWorldPosition() const {
        glm::vec3 localPos(m_LocalPosition.X(), m_LocalPosition.Y(), 1);
        glm::vec3 worldPos = m_LocalToWorld * localPos;
        return Vector2(worldPos.x, worldPos.y);
    }
    
    void TransformData::SetLocalScale(const Vector2 &scale) {
        m_LocalScale = scale;
        m_IsDirty = true;
    }
    
    Vector2 TransformData::LocalToWorldPoint(const Vector2& point) const {
        glm::vec3 point3(point.X(), point.Y(), 1);
        glm::vec3 worldPoint3 = m_LocalToWorld * point3;
        return Vector2(worldPoint3.x, worldPoint3.y);
    }
    
    void TransformData::RecalculateLocalToWorld() {
        glm::mat3 localTransform(1.0);
        
        float Sx = m_LocalScale.X();
        float Sy = m_LocalScale.Y();
        float Px = m_LocalPosition.X();
        float Py = m_LocalPosition.Y();
        localTransform[0][0] = Sx;
        localTransform[1][1] = Sy;
        localTransform[2][0] = Px;
        localTransform[2][1] = Py;
        
        if (m_Entity->GetParent() == nullptr)
            m_LocalToWorld = localTransform;
        else
            m_LocalToWorld = m_Entity->GetParent()->m_LocalToWorld * localTransform;
        
        m_IsDirty = false;
    }
}
