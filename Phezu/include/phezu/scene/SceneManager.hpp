#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Phezu {
    
    class Engine;
    class Scene;
    
    class SceneManager {
    public:
        SceneManager() = delete;
        SceneManager(Engine* engine);
    public:
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        std::weak_ptr<Scene> GetActiveScene() const;
    public:
        void OnStartGame();
        void OnEndOfFrame();
        void LoadScene(const std::string& sceneName);
        std::weak_ptr<Scene> GetMasterScene() const { return m_MasterScene; }
    private:
        Engine* m_Engine;
        std::shared_ptr<Scene> m_MasterScene;
        std::unordered_map<std::string, std::shared_ptr<Scene>> m_AllScenes;
        Scene* m_ActiveScene;
        std::string m_SceneToLoad;
        bool m_LoadSceneAfterFrame;
    };
}
