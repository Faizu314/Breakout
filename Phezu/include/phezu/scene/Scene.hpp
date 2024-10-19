#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "scene/Entity.hpp"

namespace Phezu {

    class Entity;
    
    class Scene {
    public:
        Scene(const std::string& name);
        void SetPrefabEntities(std::vector<std::unique_ptr<Entity>>& entities);
        std::weak_ptr<Entity> CreateEntity();
        void DestroyEntity(std::weak_ptr<Entity> entity);
        void Load();
        void Unload();
    private:
        const std::string m_Name;
        bool m_IsLoaded;
        std::vector<std::unique_ptr<Entity>> m_PrefabEntities;
        std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_RuntimeEntities;
    };
}
