#pragma once

#include "Renderer.hpp"

#include "scene/Scene.hpp"
#include "scene/SceneManager.hpp"
#include "scene/Entity.hpp"

#include "scene/components/DataComponent.hpp"
#include "scene/components/BehaviourComponent.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"

#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine {
    public:
        int Init();
        std::weak_ptr<Scene> CreateScene();
        bool Run();
        void Destroy();
    private:
        SceneManager m_SceneManager;
        bool m_HasInited;
    };
}
