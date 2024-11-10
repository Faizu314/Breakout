#pragma once

#include <unordered_map>

#include "Window.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "Physics.hpp"
#include "maths/Math.hpp"
#include "scene/Prefab.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/PrefabEntity.hpp"
#include "scene/EntityTemplate.hpp"
#include "scene/Entity.hpp"
#include "scene/components/DataComponent.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"
#include "scene/components/BehaviourComponentPrefab.hpp"

namespace Phezu {
    
    class Scene;
    class Prefab;
    
    class Engine {
    public:
        Engine();
        int Init();
        void CreateWindow(const std::string name, int width, int height, int renderScale);
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        void Run();
        void Destroy();
        std::weak_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
        long long unsigned int GetFrameCount() const { return m_FrameCount; }
    private:
        Window* m_Window;
        Renderer* m_Renderer;
        Input m_Input;
        SceneManager m_SceneManager;
        Physics m_Physics;
        std::unordered_map<uint64_t, std::shared_ptr<Prefab>> m_Prefabs;
        bool m_HasInited;
        long long unsigned int m_FrameCount;
    };
}
