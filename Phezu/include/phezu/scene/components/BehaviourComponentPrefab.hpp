#pragma once

#include <cstdint>
#include <memory>
#include <typeindex>

#include "scene/Scene.hpp"
#include "scene/Entity.hpp"

namespace Phezu {
    
    class BehaviourComponent;
    
    class BehaviourComponentPrefabBase {
    public:
        BehaviourComponentPrefabBase() = delete;
        BehaviourComponentPrefabBase(uint64_t prefabID, std::unique_ptr<size_t[]> pathToPrefabEntity, size_t pathSize, uint8_t componentPrefabID);
        uint64_t GetEntityPrefabID() const;
        uint8_t GetComponentID() const;
        virtual std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const = 0;
        virtual void InitRuntimeComponentInternal(std::weak_ptr<Scene> scene, std::shared_ptr<BehaviourComponent> component) const = 0;
        virtual std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<Entity> entity) const = 0;
    protected:
        const uint64_t m_PrefabID;
        const uint8_t m_ComponentPrefabID;
        const size_t m_PathSize;
        std::unique_ptr<size_t[]> m_PathToPrefabEntity;
    };
    
    template <typename T>
    class BehaviourComponentPrefab : public BehaviourComponentPrefabBase {
        static_assert(std::is_base_of<BehaviourComponent, T>::value, "Component T is not of type BehaviourComponent");
    public:
        using BehaviourComponentPrefabBase::BehaviourComponentPrefabBase;
    public:
        std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const override final {
            auto entityL = entity.lock();
            return entityL->AddComponent<T>(m_ComponentPrefabID);
        }
        void InitRuntimeComponentInternal(std::weak_ptr<Scene> scene, std::shared_ptr<BehaviourComponent> component) const override final {
            InitRuntimeComponent(scene, std::dynamic_pointer_cast<T>(component));
        }
        std::weak_ptr<BehaviourComponent> GetRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<Entity> entity) const override final {
            return entity->GetComponent<T>(m_ComponentPrefabID);
        }
        virtual void InitRuntimeComponent(std::weak_ptr<Scene> scene, std::shared_ptr<T> component) const {};
    };
}
