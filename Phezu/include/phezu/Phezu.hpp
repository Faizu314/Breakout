#pragma once

#include <unordered_map>
#include "scene/SceneManager.hpp"

namespace Phezu {
    
    class Scene;
    class EntityPrefab;
    
    class Engine {
    public:
        int Init();
        std::weak_ptr<Scene> CreateScene();
        bool Run();
        void Destroy();
        std::shared_ptr<EntityPrefab> CreateEntityPrefab();
        std::weak_ptr<const EntityPrefab> GetEntityPrefab(uint64_t prefabID);
    private:
        std::unordered_map<uint64_t, std::shared_ptr<EntityPrefab>> m_PrefabEntities;
        SceneManager m_SceneManager;
        bool m_HasInited;
    };
}
