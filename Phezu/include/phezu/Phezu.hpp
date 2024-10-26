#pragma once

#include <unordered_map>
#include "scene/SceneManager.hpp"

namespace Phezu {
    
    class Scene;
    class Prefab;
    
    class Engine {
    public:
        Engine();
        int Init();
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        bool Run();
        void Destroy();
        std::weak_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
    private:
        std::unordered_map<uint64_t, std::shared_ptr<Prefab>> m_Prefabs;
        SceneManager m_SceneManager;
        bool m_HasInited;
    };
}
