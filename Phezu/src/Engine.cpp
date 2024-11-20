#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "scene/Scene.hpp"
#include "scene/Prefab.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

namespace Phezu {
    
    static const size_t ENTITIES_BUFFER_SIZE = 128;
    
    Engine::Engine() : m_HasInited(false), m_IsRunning(false), m_FrameCount(0), m_SceneManager(this), m_Input(this), m_Physics(this) {}
    
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
        
        //atexit(Destroy);
        return 0;
    }
    
    void Engine::CreateWindow(const std::string name, int width, int height, int renderScale) {
        if (m_Window != nullptr || !m_HasInited) {
            //TODO: Logging
            return;
        }
            
        m_Window = new Window(name, width, height, renderScale);
        m_Renderer = new Renderer(this, *m_Window);
    }
    
    std::weak_ptr<Scene> Engine::CreateScene(const std::string& name) {
        if (m_IsRunning) {
            //TODO: Logging;
            return std::weak_ptr<Scene>();
        }
        if (!m_HasInited) {
            
            return std::weak_ptr<Scene>();
        }
        if (name.empty()) {
            
            return std::weak_ptr<Scene>();
        }
        
        return m_SceneManager.CreateScene(name);
    }
    
    std::weak_ptr<Scene> Engine::GetMasterScene() {
        return m_SceneManager.GetMasterScene();
    }
    
    void Engine::LoadScene(const std::string& sceneName) {
        if (!m_IsRunning) {
            //TODO: Logging
            return;
        }
        
        m_SceneManager.LoadScene(sceneName);
    }
    
    float GetDeltaTime(Uint64& prevTime, Uint64& freqMs) {
        Uint64 currTime = SDL_GetPerformanceCounter();
        float deltaTime = (currTime - prevTime) / (float)freqMs;
        prevTime = SDL_GetPerformanceCounter();
        
        return deltaTime;
    }
    
    void Engine::Run() {
        if (!m_HasInited || m_Window == nullptr || m_Renderer == nullptr) {
            //TODO: logging file
            return;
        }
        
        m_IsRunning = true;
        
        m_SceneManager.OnStartGame();
        
        Uint64 prevTime = SDL_GetPerformanceCounter();
        Uint64 freqMs = SDL_GetPerformanceFrequency();
        float deltaTime;

        std::vector<std::weak_ptr<Entity>> entitiesBuffer(ENTITIES_BUFFER_SIZE);
        size_t renderablesCount;
        size_t staticsCount;
        size_t dynamicsCount;
        
        std::shared_ptr<Scene> masterScene = m_SceneManager.GetMasterScene().lock();
        std::weak_ptr<Scene> activeScene;
        
        SDL_Event event;
        
        while (m_IsRunning)
        {
            if (!m_Input.PollInput())
                break;
            
            m_Renderer->ClearFrame();
            
            deltaTime = GetDeltaTime(prevTime, freqMs);
            
            masterScene->LogicUpdate(deltaTime);
            
            masterScene->GetRenderableEntities(entitiesBuffer, renderablesCount);
            m_Renderer->DrawEntities(entitiesBuffer, renderablesCount);
            
            activeScene = m_SceneManager.GetActiveScene();
            
            if (auto sceneL = activeScene.lock()) {
                sceneL->LogicUpdate(deltaTime);
                
                sceneL->GetPhysicsEntities(entitiesBuffer, staticsCount, dynamicsCount);
                m_Physics.PhysicsUpdate(entitiesBuffer, staticsCount, dynamicsCount, deltaTime);
                
                sceneL->GetRenderableEntities(entitiesBuffer, renderablesCount);
                m_Renderer->DrawEntities(entitiesBuffer, renderablesCount);
            }
            
            m_Renderer->RenderFrame();
            
            m_SceneManager.OnEndOfFrame();
            
            m_FrameCount++;
        }
        
        m_IsRunning = false;
        
        Destroy();
    }
    
    void Engine::Destroy() {
        delete m_Renderer;
        delete m_Window;
    }
    
    std::weak_ptr<Prefab> Engine::CreatePrefab() {
        if (!m_HasInited) {
            //TODO: Logging
            return std::weak_ptr<Prefab>();
        }
        
        std::shared_ptr<Prefab> prefab = std::make_shared<Prefab>();
        m_Prefabs.insert(std::make_pair(prefab->RootEntity.GetPrefabEntityID(), prefab));
        return prefab;
    }
    
    std::weak_ptr<const Prefab> Engine::GetPrefab(uint64_t prefabID) {
        if (!m_HasInited) {
            //TODO: Logging
            return std::weak_ptr<const Prefab>();
        }
        
        if (m_Prefabs.find(prefabID) == m_Prefabs.end())
            return std::weak_ptr<const Prefab>();
        return m_Prefabs[prefabID];
    }
}
