#pragma once

#include <memory>
#include <vector>

namespace Phezu {
    
    class Engine;
    class Scene;
    
    class SceneManager {
    public:
        SceneManager() = delete;
        SceneManager(Engine* engine);
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        std::weak_ptr<Scene> GetActiveScene() const;
    public:
        void OnStartGame(); //load the scene at index 0
        void LoadScene(std::weak_ptr<Scene> scene) const;
        void UpdateScene(float deltaTime);
    private:
        Engine* m_Engine;
        std::vector<std::shared_ptr<Scene>> m_AllScenes;
        int m_ActiveSceneIndex;
    };
}
