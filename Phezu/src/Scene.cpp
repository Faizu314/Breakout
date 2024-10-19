#include "scene/Scene.hpp"

namespace Phezu {
    
    Scene::Scene(const std::string& name) : m_Name(name) { }
    
    void Scene::SetPrefabEntities(std::vector<std::unique_ptr<Entity>>& entities) {
        m_PrefabEntities = std::move(entities);
    }
    
    std::weak_ptr<Entity> Scene::CreateEntity() {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(std::shared_ptr<Scene>(this));
        m_RuntimeEntities.insert(std::make_pair(entity->GetID(), entity));
        
        return entity;
    }
    
    void Scene::DestroyEntity(std::weak_ptr<Entity> entity) {
        auto lockedEntity = entity.lock();
        
        if (!lockedEntity)
            return;
        
        uint64_t id = lockedEntity->GetID();
        auto it = m_RuntimeEntities.find(id);
        
        if (it == m_RuntimeEntities.end())
            return;
        
        size_t childCount = it->second->GetChildCount();
        
        for (size_t i = 0; i < childCount; i++)
            DestroyEntity(it->second->GetChild(i));
            
        m_RuntimeEntities.erase(it);
    }
    
    void Scene::Load() {
        
        
        m_IsLoaded = true;
    }
    
    void Scene::Unload() {
        
        
        m_IsLoaded = false;
    }
}
