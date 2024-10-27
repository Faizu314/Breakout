#include "scene/Entity.hpp"
#include "scene/components/TransformData.hpp"

namespace Phezu {

    TransformData::TransformData(Entity* entity) : DataComponent(entity) {}
    
    void TransformData::SetLocalPosition(const Vector2& position) {
        m_LocalPosition = position;
        m_IsDirty = true;
    }
    
    void TransformData::SetScale(const Vector2 &scale) {
        m_Scale = scale;
        m_IsDirty = true;
    }
    
    void TransformData::RecalculateTransformations() {
        RecalculateLocalToParent();
        RecalculateLocalToWorld();
        m_IsDirty = false;
    }
    
    void TransformData::RecalculateLocalToParent() {
        m_LocalToParent = glm::mat3(1.0);
        
        if (m_Entity->GetParent() == nullptr)
            return;
        
        TransformData* parent = m_Entity->GetParent();
        float Sx = parent->GetScale().X();
        float Sy = parent->GetScale().Y();
        float Px = parent->GetLocalPosition().X();
        float Py = parent->GetLocalPosition().Y();
        m_LocalToParent[0][0] = Sx;
        m_LocalToParent[1][1] = Sy;
        m_LocalToParent[2][0] = -Px * Sx;
        m_LocalToParent[2][1] = -Py * Sy;
    }
    
    void TransformData::RecalculateLocalToWorld() {
        if (m_Entity->GetParent() == nullptr)
            m_LocalToWorld = m_LocalToParent;
        else
            m_LocalToWorld = m_Entity->GetParent()->m_LocalToWorld * m_LocalToParent;
    }
}
