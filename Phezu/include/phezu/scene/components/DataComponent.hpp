#pragma once

#include "scene/Entity.hpp"

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent(const std::weak_ptr<Entity>& entity) : m_Entity(entity) {}
        std::weak_ptr<Entity> GetEntity() const { return m_Entity; }
    protected:
        std::weak_ptr<Entity> m_Entity;
    };
}
