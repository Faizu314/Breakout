#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class TransformData : public DataComponent {
    public:
        TransformData(Entity* entity);
    public:
        Vector2 GetPosition() { return m_LocalPosition; }
        Vector2 GetScale() { return m_Scale; }
        void SetPosition(const Vector2& position);
        void SetScale(const Vector2& scale);
        const glm::mat3& GetLocalToParent() { return m_LocalToParent; }
        const glm::mat3& GetLocalToWorld() { return m_LocalToWorld; }
        void RecalculateTransformations();
        bool GetIsDirty() { return m_IsDirty; }
    private:
        void RecalculateLocalToParent();
        void RecalculateLocalToWorld();
    private:
        Vector2 m_LocalPosition;
        Vector2 m_Scale;
        glm::mat3 m_LocalToParent;
        glm::mat3 m_LocalToWorld;
        bool m_IsDirty;
    };
}
