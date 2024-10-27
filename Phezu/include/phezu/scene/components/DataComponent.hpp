#pragma once

namespace Phezu {
    
    class Entity;
    
    class DataComponent {
    public:
        DataComponent() = delete;
        DataComponent(Entity* entity) : m_Entity(entity) {}
        Entity* GetEntity() const { return m_Entity; }
    protected:
        Entity* const m_Entity;
    };
}
