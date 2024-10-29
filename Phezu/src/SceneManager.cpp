#include "scene/SceneManager.hpp"
#include "scene/Scene.hpp"
#include "scene/EntityTemplate.hpp"

namespace Phezu {
    
    SceneManager::SceneManager(Engine* engine) : m_Engine(engine), m_ActiveSceneIndex(0) { }
    
    std::weak_ptr<Scene> SceneManager::CreateScene(const std::string& name) {
        std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_Engine, name);
        m_AllScenes.push_back(scene);
        return scene;
    }

    void SceneManager::OnStartGame() {
        if (m_AllScenes.size() == 0) {
            //TODO: logging
            return;
        }
        LoadScene(m_AllScenes[0]);
    }
    
    void SceneManager::LoadScene(std::weak_ptr<Scene> scene) const {
        scene.lock()->Load();
    }
    
    void SceneManager::UpdateScene(float deltaTime) {
        m_AllScenes[m_ActiveSceneIndex]->LogicUpdate(deltaTime);
    }
    
    std::weak_ptr<Scene> SceneManager::GetActiveScene() const {
        return m_AllScenes[m_ActiveSceneIndex];
    }
}
