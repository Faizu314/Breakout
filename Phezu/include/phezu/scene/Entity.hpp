#pragma once

#include <vector>

#include "scene/Scene.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"

namespace Phezu {
    
    class Scene;
    
    class Entity {
    public:
        Entity(const std::weak_ptr<Scene> scene);
        ~Entity();
        uint64_t GetID();
        void SetActive(bool isActive);
        bool GetActive() const;
        TransformData& GetTransformData();
        Rect& GetShapeData();
        RenderData& GetRenderData();
        PhysicsData& GetPhysicsData();
        TransformData& GetParent();
        size_t GetChildCount();
        std::weak_ptr<Entity> GetChild(size_t childIndex);
        
        template<typename T>
        std::weak_ptr<T> GetComponent();
        template<typename T>
        std::weak_ptr<T> AddComponent();
        template<typename T>
        void RemoveComponent();
        
    private:
        void OnChildDestroyed();
    private:
        const std::weak_ptr<Scene> m_Scene;
        TransformData* m_Parent;
        std::vector<std::weak_ptr<Entity>> m_Children;
    private:
        TransformData m_TransformData;
        Rect m_ShapeData;
        RenderData m_RenderData;
        PhysicsData* m_PhysicsData;
    private:
        std::vector<std::shared_ptr<BehaviourComponent>> m_BehaviourComponents;
    private:
        static uint64_t s_EntitiesCount;
        uint64_t m_EntityID;
        bool m_IsActive;
    };
    
    uint64_t Entity::s_EntitiesCount = 0;
}
