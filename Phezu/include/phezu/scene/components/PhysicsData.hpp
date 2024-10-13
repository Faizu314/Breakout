#pragma once

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class PhysicsData : public DataComponent {
    public:
        Vector2 Velocity;
        bool IsStatic() const;
        PhysicsData(const std::weak_ptr<Entity>& entity, bool isStatic) : DataComponent(entity), m_IsStatic(isStatic) {}
    private:
        const bool m_IsStatic;
    };
}
