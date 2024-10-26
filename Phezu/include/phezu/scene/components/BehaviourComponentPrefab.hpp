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
        BehaviourComponentPrefabBase(uint64_t prefabID, std::unique_ptr<size_t[]> pathToPrefabEntity, size_t pathSize, uint8_t componentID);
        uint64_t GetEntityPrefabID() const;
        uint8_t GetComponentID() const;
        virtual std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const = 0;
        virtual void InitRuntimeComponent(std::weak_ptr<Scene> scene, uint64_t instanceID) const = 0;
        virtual bool IsOfType(std::type_index type) const = 0;
    protected:
        const uint64_t m_PrefabID;
        const uint8_t m_ComponentID;
        const size_t m_PathSize;
        std::unique_ptr<size_t[]> m_PathToPrefabEntity;
    };
    
    template <typename T>
    class BehaviourComponentPrefab : public BehaviourComponentPrefabBase {
    public:
        using BehaviourComponentPrefabBase::BehaviourComponentPrefabBase;
    public:
        std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const override {
            auto entityL = entity.lock();
            return entityL->AddComponent<T>();
        }
        bool IsOfType(std::type_index type) const override {
            return std::type_index(typeid(T)) == type;
        }
    protected:
        std::weak_ptr<Entity> GetRuntimeEntity(std::weak_ptr<Scene> scene, uint64_t instanceID) {
            auto sceneL = scene.lock();
            std::weak_ptr<const Prefab> prefab = sceneL->GetPrefab(m_PrefabID);
            std::shared_ptr<Entity> rootEntity = sceneL->GetRuntimeEntityFromSceneEntity(instanceID).lock();
            
            std::shared_ptr<Entity> targetEntity = rootEntity;
            for (size_t i = 0; i < m_PathSize; i++) {
                targetEntity = targetEntity->GetChild(m_PathToPrefabEntity[i]).lock();
            }
            
            return targetEntity;
        }
        std::weak_ptr<T> GetRuntimeComponent(std::weak_ptr<Scene> scene, uint64_t instanceID) {
            std::weak_ptr<Entity> targetEntity = GetRuntimeEntity(scene, instanceID);
            std::shared_ptr<Entity> targetEntityL = targetEntity.lock();
            
            //TODO: Get the right component if there are multiple components of the same type attached
            return targetEntityL->GetComponent<T>();
        }
    };
}
