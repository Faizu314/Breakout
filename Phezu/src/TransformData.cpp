#include "scene/Entity.hpp"
#include "scene/components/TransformData.hpp"

namespace Phezu {

    TransformData::TransformData(Entity* entity) : DataComponent(entity) {}
    
    void TransformData::SetPosition(const Vector2& position) {
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
        
    }
    
    void TransformData::RecalculateLocalToWorld() {
        
    }
}
