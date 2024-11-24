#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
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
        template <typename T>
        void SubscribeToOnSceneLoaded(T* subscriber, void (T::*handler)(void)) {
            if (m_OnSceneLoaded.find(subscriber) != m_OnSceneLoaded.end())
                return;
            
            auto delegate = [subscriber, handler](void) {
                (subscriber->*handler)();
            };
            
            m_OnSceneLoaded.insert(std::make_pair(subscriber, delegate));
        }
        void UnsubscribeToOnSceneLoaded(void* subscriber);
    public:
        void OnStartGame();
        void OnEndOfFrame();
        void LoadScene(const std::string& sceneName);
        std::weak_ptr<Scene> GetMasterScene() const { return m_MasterScene; }
    private:
        Engine* m_Engine;
    private:
        std::shared_ptr<Scene> m_MasterScene;
        std::unordered_map<std::string, std::shared_ptr<Scene>> m_AllScenes;
    private:
        Scene* m_ActiveScene;
        std::string m_SceneToLoad;
        bool m_LoadSceneAfterFrame;
    private:
        std::unordered_map<void*, std::function<void()>> m_OnSceneLoaded;
    };
}
