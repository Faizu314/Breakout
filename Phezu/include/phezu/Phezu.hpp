#pragma once

#include <unordered_map>
#include "scene/SceneManager.hpp"

namespace Phezu {
    
    class Scene;
    class Prefab;
    
    class Engine {
    public:
        int Init();
        std::weak_ptr<Scene> CreateScene();
        bool Run();
        void Destroy();
        std::shared_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
    private:
        std::unordered_map<uint64_t, std::shared_ptr<Prefab>> m_PrefabEntities;
        SceneManager m_SceneManager;
        bool m_HasInited;
    };
}
