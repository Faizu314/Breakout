#pragma once

#include <functional>
#include <unordered_map>
#include <memory>

#include "maths/Math.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class Physics;
    class BehaviourComponent;
    class PhysicsData;
    
    struct Collision {
        Collision(std::weak_ptr<PhysicsData> other) : Other(other) {}
        std::weak_ptr<PhysicsData> Other;
        Vector2 Point;
        Vector2 Normal;
    };
    
    class PhysicsData : public DataComponent {
    public:
        PhysicsData(Entity* entity, bool isStatic) : DataComponent(entity), m_IsStatic(isStatic) {}
    public:
        bool IsStatic() const { return m_IsStatic; }
    private:
        void OnCollisionEnter(const Collision& collision);
        void OnCollisionStay(const Collision& collision);
        void OnCollisionExit(const Collision& collision);
    public:
        template <typename T>
        void RegisterOnCollisionEnter(T* instance, void (T::*handler)(const Collision&)) {
            if (m_OnCollisionEnterEvent.find(instance) != m_OnCollisionEnterEvent.end())
                return;
            
            auto delegate = [instance, handler](const Collision& collision) {
                (instance->*handler)(collision);
            };
            
            m_OnCollisionEnterEvent.insert(std::make_pair(instance, delegate));
        }
        template <typename T>
        void RegisterOnCollisionStay(T* instance, void (T::*handler)(const Collision&)) {
            if (m_OnCollisionStayEvent.find(instance) != m_OnCollisionStayEvent.end())
                return;
            
            auto delegate = [instance, handler](const Collision& collision) {
                (instance->*handler)(collision);
            };
            
            m_OnCollisionStayEvent.insert(std::make_pair(instance, delegate));
        }
        template <typename T>
        void RegisterOnCollisionExit(T* instance, void (T::*handler)(const Collision&)) {
            if (m_OnCollisionExitEvent.find(instance) != m_OnCollisionExitEvent.end())
                return;
            
            auto delegate = [instance, handler](const Collision& collision) {
                (instance->*handler)(collision);
            };
            
            m_OnCollisionExitEvent.insert(std::make_pair(instance, delegate));
        }
    public:
        void UnregisterOnCollisionEnter(BehaviourComponent* comp);
        void UnregisterOnCollisionStay(BehaviourComponent* comp);
        void UnregisterOnCollisionExit(BehaviourComponent* comp);
    public:
        Vector2 Velocity;
    private:
        const bool m_IsStatic;
        std::unordered_map<void*, std::function<void(const Collision&)>> m_OnCollisionEnterEvent;
        std::unordered_map<void*, std::function<void(const Collision&)>> m_OnCollisionStayEvent;
        std::unordered_map<void*, std::function<void(const Collision&)>> m_OnCollisionExitEvent;
        
        friend Physics;
    };
}
