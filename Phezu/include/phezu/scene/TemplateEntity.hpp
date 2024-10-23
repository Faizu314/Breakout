#pragma once

#include <vector>

namespace Phezu {
    
    class Scene;
    class Entity;
    class PrefabEntity;
    class BehaviourComponent;
    class BehaviourComponentPrefab;
    
    class TemplateEntity {
    private:
        struct ConstructionToken {};
        static std::shared_ptr<TemplateEntity> MakeShared(std::weak_ptr<Scene> scene, uint64_t prefabID = 0, uint64_t instanceID = 0);
        static std::unique_ptr<TemplateEntity> MakeUnique(std::weak_ptr<Scene> scene, uint64_t prefabID = 0, uint64_t instanceID = 0);
    public:
        //if hierarchy entity is not from a prefab than use a default prefab template
        //EntityPrefabOverride OverridePrefab();
        TemplateEntity(ConstructionToken token, std::weak_ptr<Scene> scene, uint64_t prefabID = 0, uint64_t instanceID = 0);
    public:
        uint64_t GetPrefabID() const;
        uint64_t GetInstanceID() const;
        std::weak_ptr<Entity> GetRuntimeEntity(PrefabEntity& entityPrefab);
        std::weak_ptr<BehaviourComponent> GetRuntimeBehaviourComponent(BehaviourComponentPrefab& componentPrefab);
    private:
        const std::weak_ptr<Scene> m_Scene;
        const uint64_t m_PrefabID;
        const uint64_t m_InstanceID;
        std::weak_ptr<Entity> m_RuntimeEntity;
        
        friend class Scene;
    };
}
