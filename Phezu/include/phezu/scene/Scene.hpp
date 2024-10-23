#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

namespace Phezu {
    
    class Engine;
    class Entity;
    class PrefabEntity;
    class TemplateEntity;
    
    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene(Engine* engine, const std::string& name);
        void CreateHierarchyEntity(uint64_t entityPrefabID = 0);
        std::weak_ptr<Entity> CreateEntity();
        void DestroyEntity(uint64_t entityID);
        void Load();
        void Unload();
    private:
        void BuildEntityFromPrefab(std::shared_ptr<Entity> entity, std::unique_ptr<TemplateEntity>& prefab);
        void MoveRefsFromPrefabToEntity(std::shared_ptr<Entity>& entity, std::unique_ptr<TemplateEntity>& prefab);
    private:
        Engine* const m_Engine;
        const std::string m_Name;
        bool m_IsLoaded;
        std::vector<std::unique_ptr<TemplateEntity>> m_HierarchyEntities;
        std::unordered_map<uint64_t, std::shared_ptr<Entity>> m_RuntimeEntities;
    };
}
