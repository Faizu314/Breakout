#pragma once

#include "scene/Entity.hpp"

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent(Entity* entity) : m_Entity(entity) {}
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
    };
}
