#pragma once

#include <vector>
#include <typeindex>

#include "Renderer.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/BehaviourComponent.hpp"

namespace Phezu {
    
    class RenderData;
    class PhysicsData;
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
    public:
        template<typename T>
        std::weak_ptr<T> GetComponent() {
            if (!std::is_base_of<T, BehaviourComponent>::value) {
                //TODO: copy and paste the logging class
                return;
            }
            
            for (int i = 0; i < m_BehaviourComponents.size(); i++) {
                auto* componentPtr = m_BehaviourComponents[i].get();
                if (typeid(*componentPtr) == typeid(T)) {
                    return m_BehaviourComponents[i];
                }
            }
        }
        
        template<typename T>
        std::weak_ptr<T> AddComponent() {
            static_assert(!std::is_base_of<T, BehaviourComponent>::value, "Component T is not of type BehaviourComponent");
            
            uint8_t componentID = m_BehaviourComponents.size(); //TODO: This should be the count of other BehaviourComponent that are of the same type as T
            
            std::shared_ptr<T> component = std::make_shared<T>(this, componentID);
            
            m_BehaviourComponents.push_back(std::static_pointer_cast<BehaviourComponent>(component));
            
            return component;
        }
        
        template<typename T>
        void RemoveComponent() {
            if (!std::is_base_of<T, BehaviourComponent>::value) {
                //TODO: copy and paste the logging class
                return;
            }
            
            for (int i = 0; i < m_BehaviourComponents.size(); i++) {
                auto* componentPtr = m_BehaviourComponents[i].get();
                if (typeid(*componentPtr) == typeid(T)) {
                    m_BehaviourComponents.erase(m_BehaviourComponents.begin() + i);
                    return;
                }
            }
        }
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
