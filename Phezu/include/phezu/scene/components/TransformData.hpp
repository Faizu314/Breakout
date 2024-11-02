#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class TransformData : public DataComponent {
    public:
        TransformData(Entity* entity);
    public:
        Vector2 GetLocalPosition() { return m_LocalPosition; }
        Vector2 GetWorldPosition() const;
        Vector2 GetLocalScale() { return m_LocalScale; }
        void SetLocalPosition(const Vector2& position);
        void SetLocalScale(const Vector2& scale);
        Vector2 LocalToWorldPoint(const Vector2& point) const;
    public:
        void RecalculateLocalToWorld();
        bool GetIsDirty() { return m_IsDirty; }
    private:
        Vector2 m_LocalPosition;
        Vector2 m_LocalScale;
        glm::mat3 m_LocalToWorld;
        bool m_IsDirty;
    };
}
