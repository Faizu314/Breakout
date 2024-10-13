#pragma once

#include <vector>
#include <memory>

#include "scene/Entity.hpp"

namespace Phezu {
    
    class Scene {
    public:
        std::weak_ptr<Entity> CreateEntity();
        void DestroyEntity(std::weak_ptr<Entity> entity);
        void Load();
        void Unload();
    private:
        bool m_IsLoaded;
        std::vector<std::shared_ptr<Entity>> m_PrefabEntities;
        std::vector<std::shared_ptr<Entity>> m_RuntimeEntities;
    };
}
