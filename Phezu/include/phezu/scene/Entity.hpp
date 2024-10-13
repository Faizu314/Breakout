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
        void SetActive(bool isActive);
        bool GetActive() const;
        TransformData& GetTransformData() const;
        std::weak_ptr<TransformData> GetParent() const;
        Rect& GetShapeData() const;
        RenderData& GetRenderData() const;
        PhysicsData& GetPhysicsData() const;
        
        template<typename T>
        T GetComponent() const;
        template<typename T>
        T AddComponent();
        template<typename T>
        void RemoveComponent(T component);
        //static_assert(std::is_base_of<BaseClass, Derived>::value, "Derived not derived from BaseClass");
        
    private:
        TransformData m_TransformData;
        Rect m_ShapeData;
        RenderData m_RenderData;
        PhysicsData m_PhysicsData;
    private:
        std::weak_ptr<TransformData> m_Parent;
        bool m_IsActive;
    private:
        std::vector<std::shared_ptr<BehaviourComponent>> m_BehaviourComponents;
    };
}
