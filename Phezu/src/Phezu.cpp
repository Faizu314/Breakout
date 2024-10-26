#include "Phezu.hpp"
#include "scene/SceneManager.hpp"
#include "scene/Prefab.hpp"

namespace Phezu {
    
    Engine::Engine() : m_HasInited(false), m_SceneManager(this) { }
    
    int Engine::Init() {
        m_HasInited = true;
    }
    
    std::weak_ptr<Scene> Engine::CreateScene(const std::string& name) {
        return m_SceneManager.CreateScene(name);
    }
    
    bool Engine::Run() {
        bool isRunning = true;
        
        m_SceneManager.OnStartGame();
    }
    
    void Engine::Destroy() {
        //m_SceneManager.Cleanup()
    }
    
    std::weak_ptr<Prefab> Engine::CreatePrefab() {
        std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>();
        m_Prefabs.insert(std::make_pair(prefab->RootEntity.GetPrefabEntityID(), prefab));
        return prefab;
    }
    
    std::weak_ptr<const Prefab> Engine::GetPrefab(uint64_t prefabID) {
        if (m_Prefabs.find(prefabID) == m_Prefabs.end())
            return std::weak_ptr<const Prefab>();
        return m_Prefabs[prefabID];
    }
}
