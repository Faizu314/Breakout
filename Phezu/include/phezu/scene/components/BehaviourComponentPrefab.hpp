#pragma once

#include <cstdint>
#include <memory>
#include <typeindex>

namespace Phezu {
    
    class Scene;
    class Entity;
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
        std::weak_ptr<BehaviourComponent> CreateComponent(std::weak_ptr<Entity> entity) const override;
        bool IsOfType(std::type_index type) const override;
    protected:
        std::weak_ptr<T> GetRuntimeComponent(std::weak_ptr<Scene> scene, uint64_t instanceID);
        std::weak_ptr<Entity> GetRuntimeEntity(std::weak_ptr<Scene> scene, uint64_t instanceID);
    };
}
