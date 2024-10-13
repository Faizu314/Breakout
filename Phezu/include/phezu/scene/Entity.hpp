#pragma once

#include <vector>

#include "scene/components/TransformData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"

namespace Phezu {
    
    class Entity {
    public:
        Entity();
        ~Entity();
        TransformData& GetTransformData();
        TransformData& GetParent();
        Rect& GetShapeData();
        RenderData& GetRenderData();
        PhysicsData& GetPhysicsData();
        
        template<typename T>
        T GetComponent();
        template<typename T>
        T AddComponent();
        template<typename T>
        void RemoveComponent(T component);
        //static_assert(std::is_base_of<BaseClass, Derived>::value, "Derived not derived from BaseClass");
        
    private:
        TransformData m_TransformData;
        TransformData m_Parent;
        Rect m_ShapeData;
        RenderData m_RenderData;
        PhysicsData m_PhysicsData;
        std::vector<std::shared_ptr<BehaviourComponent>> m_BehaviourComponents;
    };
}
