#pragma once

#include <vector>
#include <typeindex>

#include "Renderer.hpp"
#include "scene/components/TransformData.hpp"

namespace Phezu {
    
    class RenderData;
    class PhysicsData;
    class BehaviourComponent;
    class Scene;
    
    class Entity {
    public:
        Entity() = delete;
        Entity(const std::weak_ptr<Scene> scene);
        ~Entity();
        uint64_t GetEntityID() const;
        void SetActive(bool isActive);
        bool GetActive() const;
        TransformData& GetTransformData();
        Rect* const AddShapeData();
        RenderData* const AddRenderData(Color tint = Colors::WHITE);
        PhysicsData* const AddPhysicsData(bool isStatic);
        TransformData* const GetParent();
        void SetParent(std::weak_ptr<Entity> parent);
        void RemoveParent();
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
        void AddChild(std::weak_ptr<Entity> child);
    private:
        const std::weak_ptr<Scene> m_Scene;
        TransformData* m_Parent;
        std::vector<std::weak_ptr<Entity>> m_Children;
    private:
        TransformData m_TransformData;
        Rect* m_ShapeData;
        RenderData* m_RenderData;
        PhysicsData* m_PhysicsData;
    private:
        std::vector<std::shared_ptr<BehaviourComponent>> m_BehaviourComponents;
    private:
        static uint64_t s_EntitiesCount;
        uint64_t m_EntityID;
        bool m_IsActive;
        
        friend void SetParent_Internal(std::weak_ptr<Entity> _this, std::weak_ptr<Entity> child);
    };
}
