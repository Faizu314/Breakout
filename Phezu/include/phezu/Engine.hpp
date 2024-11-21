#pragma once

#include <unordered_map>

#include "Input.hpp"
#include "Physics.hpp"
#include "scene/SceneManager.hpp"

namespace Phezu {
    
    class Scene;
    class Prefab;
    class Window;
    class Renderer;
    
    class Engine {
    private:
        Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;
    public:
        int Init();
        void Run();
        std::weak_ptr<Scene> GetMasterScene();
        void LoadScene(const std::string& sceneName);
        void Destroy();
        void CreateWindow(const std::string name, int width, int height, int renderScale = 1);
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        std::weak_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
        long long unsigned int GetFrameCount() const { return m_FrameCount; }
    private:
        Window* m_Window;
        Renderer* m_Renderer;
        Input m_Input;
        SceneManager m_SceneManager;
        Physics m_Physics;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<Prefab>> m_Prefabs;
        bool m_HasInited;
        bool m_IsRunning;
        long long unsigned int m_FrameCount;
        
        friend Engine& CreateEngine();
    };
}
