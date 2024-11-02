#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine;
    class Entity;
    class Prefab;
    class PrefabEntity;
    class EntityTemplate;
    
    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene() = delete;
        Scene(Engine* engine, const std::string& name);
        std::weak_ptr<const Prefab> GetPrefab(uint64_t prefabID);
        void CreateSceneEntity(uint64_t prefabEntityID = 0, Vector2 positionOverride = Vector2());
        std::weak_ptr<Entity> GetRuntimeEntityFromSceneEntity(uint64_t instanceID);
        std::weak_ptr<Entity> CreateEntity();
        std::weak_ptr<Entity> GetEntity(uint64_t entityID) const;
        void DestroyEntity(uint64_t entityID);
        void Load();
        void LogicUpdate(float deltaTime);
        void GetRenderableEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& count) const;
        void Unload();
    private:
        void BuildEntityFromTemplate(std::shared_ptr<Entity> entity, std::unique_ptr<EntityTemplate>& entityTemplate);
        void BuildEntityFromPrefabEntity(std::shared_ptr<Entity> entity, const PrefabEntity* prefabEntity);
        void ApplyTemplateOverridesToEntity(std::shared_ptr<Entity> entity, std::unique_ptr<EntityTemplate>& entityTemplate);
        void ApplyPrefabOverridesToEntity(std::shared_ptr<Entity> entity, const PrefabEntity* prefabEntity, uint64_t instanceID);
    private:
        Engine* const m_Engine;
        const std::string m_Name;
        bool m_IsLoaded;
        bool m_IsSceneToRuntimeMappingValid;
        std::vector<std::unique_ptr<EntityTemplate>> m_SceneEntities;
        std::unordered_map<uint64_t, uint64_t> m_SceneToRuntimeEntity;
        std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_RuntimeEntities;
    };
}
