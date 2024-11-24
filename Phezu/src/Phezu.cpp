#include "Phezu.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

namespace Phezu {
    
    Engine& CreateEngine() {
        if (s_Instance == nullptr)
            s_Instance = new Engine();
        
        return *s_Instance;
    }
    
    std::weak_ptr<Entity> CreateEntity() {
        if (s_Instance == nullptr) {
            //TODO: Logging
            return std::weak_ptr<Entity>();
        }
        
        auto scene = s_Instance->m_SceneManager.GetActiveScene();
        
        if (auto sceneL = scene.lock())
            return sceneL->CreateEntity();
        else
            return std::weak_ptr<Entity>();
    }
    
    std::weak_ptr<Entity> CreateEntity(uint64_t prefabID) {
        if (s_Instance == nullptr) {
            //TODO: Logging
            return std::weak_ptr<Entity>();
        }
        
        auto scene = s_Instance->m_SceneManager.GetActiveScene();
        
        if (auto sceneL = scene.lock())
            return sceneL->CreateEntity(prefabID);
        else
            return std::weak_ptr<Entity>();
    }
    
    void LoadScene(const std::string& sceneName) {
        if (s_Instance == nullptr) {
            //TODO: Logging
            return;
        }
        
        s_Instance->LoadScene(sceneName);
    }
    
    void Destroy(Entity* entity) {
        if (entity == nullptr)
            return;
        if (auto scene = entity->m_Scene.lock())
            scene->DestroyEntity(entity->GetEntityID());
    }
    
    const InputData& GetInput() {
        return s_Instance->m_Input.GetInput();
    }
    
    long long unsigned int GetFrameCount() {
        if (s_Instance == nullptr) {
            //TODO: Logging
            return 0;
        }
        
        return s_Instance->GetFrameCount();
    }
    
    void UnsubscribeToOnSceneLoaded(void* subscriber) {
        s_Instance->m_SceneManager.UnsubscribeToOnSceneLoaded(subscriber);
    }
}
