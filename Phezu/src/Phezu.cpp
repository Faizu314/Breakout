#include "Phezu.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

namespace Phezu {
    
    Engine::Engine() : m_HasInited(false), m_SceneManager(this) { }
    
    int Engine::Init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            //TODO: Logging::Log("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0){
            //TODO: Logging::Log("Couldn't initialize SDL: %s\n", SDL_GetError());
            exit(1);
        }

        if (TTF_Init() < 0) {
            //TODO: Logging::Log("Failed to init TTF: %s\n", SDL_GetError());
            exit(1);
        }
        
        m_HasInited = true;
    }
    
    void Engine::CreateWindow(const std::string name, int width, int height) {
        if (m_Window != nullptr) {
            //TODO: Logging
            return;
        }
            
        m_Window = new Window(name, width, height);
        m_Renderer = new Renderer(*m_Window);
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
        delete m_Renderer;
        delete m_Window;
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
