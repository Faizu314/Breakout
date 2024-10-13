#pragma once

#include <memory>
#include "scene/Entity.hpp"

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent(std::shared_ptr<Entity> entity) : m_Entity(entity) {}
        std::weak_ptr<Entity> GetEntity() { return m_Entity; }
    protected:
        std::shared_ptr<Entity> m_Entity;
    };
}
