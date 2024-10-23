#pragma once

#include <unordered_map>
#include "scene/SceneManager.hpp"

namespace Phezu {
    
    class Scene;
    class PrefabEntity;
    
    class Engine {
    public:
        int Init();
        std::weak_ptr<Scene> CreateScene();
        bool Run();
        void Destroy();
        std::shared_ptr<PrefabEntity> CreateEntityPrefab();
        std::weak_ptr<const PrefabEntity> GetEntityPrefab(uint64_t prefabID);
    private:
        std::unordered_map<uint64_t, std::shared_ptr<PrefabEntity>> m_PrefabEntities;
        SceneManager m_SceneManager;
        bool m_HasInited;
    };
}
