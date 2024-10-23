#pragma once

#include <vector>

namespace Phezu {
    
    class Scene;
    class Entity;
    class BehaviourComponent;
    class BehaviourComponentPrefab;
    
    class HierarchyEntity {
    private:
        struct ConstructionToken {};
        static std::shared_ptr<HierarchyEntity> MakeShared(std::weak_ptr<Scene> scene, uint64_t prefabID = 0, uint64_t instanceID = 0);
        static std::unique_ptr<HierarchyEntity> MakeUnique(std::weak_ptr<Scene> scene, uint64_t prefabID = 0, uint64_t instanceID = 0);
    public:
        //if hierarchy entity is not from a prefab than use a default prefab template
        //EntityPrefabOverride OverridePrefab();
        HierarchyEntity(ConstructionToken token, std::weak_ptr<Scene> scene, uint64_t prefabID = 0, uint64_t instanceID = 0);
    public:
        uint64_t GetPrefabID() const;
        uint64_t GetInstanceID() const;
        std::weak_ptr<Entity> GetRuntimeEntity(EntityPrefab& entityPrefab);
        std::weak_ptr<BehaviourComponent> GetRuntimeBehaviourComponent(BehaviourComponentPrefab& componentPrefab);
    private:
        const std::weak_ptr<Scene> m_Scene;
        const uint64_t m_PrefabID;
        const uint64_t m_InstanceID;
        std::weak_ptr<Entity> m_RuntimeEntity;
        
        friend class Scene;
    };
}
