#pragma once

#include <unordered_map>

#include "Renderer.hpp"
#include "maths/Math.hpp"
#include "scene/Prefab.hpp"
#include "scene/Scene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/PrefabEntity.hpp"
#include "scene/EntityTemplate.hpp"
#include "scene/Entity.hpp"
#include "scene/components/DataComponent.hpp"
#include "scene/components/TransformData.hpp"
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
        std::weak_ptr<Scene> CreateScene(const std::string& name);
        bool Run();
        void Destroy();
        std::weak_ptr<Prefab> CreatePrefab();
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
    private:
        std::unordered_map<uint64_t, std::shared_ptr<Prefab>> m_Prefabs;
        SceneManager m_SceneManager;
        bool m_HasInited;
    };
}
