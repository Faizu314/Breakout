#pragma once

#include "Engine.hpp"
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
    
    inline Engine* s_Instance;
    
    Engine& CreateEngine();
    void LoadScene(const std::string& sceneName);
    void Destroy(Entity* entity);
    long long unsigned int GetFrameCount();
    
    template<typename T>
    void SubscribeToOnSceneLoaded(T* subscriber, void (T::*handler)(void)) {
        s_Instance->m_SceneManager.SubscribeToOnSceneLoaded(subscriber, handler);
    }
    
    void UnsubscribeToOnSceneLoaded(void* subscriber);
}
