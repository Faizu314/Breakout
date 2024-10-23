#include "scene/Scene.hpp"
#include "Phezu.hpp"
#include "scene/Entity.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/BehaviourComponent.hpp"
#include "scene/PrefabEntity.hpp"
#include "scene/components/BehaviourComponentPrefab.hpp"
#include "scene/TemplateEntity.hpp"

namespace Phezu {
    
    Scene::Scene(Engine* engine, const std::string& name) : m_Engine(engine), m_Name(name) { }
    
    std::weak_ptr<Entity> Scene::CreateEntity() {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(shared_from_this());
        m_RuntimeEntities.insert(std::make_pair(entity->GetEntityID(), entity));
        
        return entity;
    }
    
    void Scene::BuildEntityFromPrefab(std::shared_ptr<Entity> entity, std::unique_ptr<TemplateEntity>& prefab) {
        std::shared_ptr<const PrefabEntity> parentPrefab = m_Engine->GetEntityPrefab(prefab->GetPrefabID()).lock();
        
        entity->GetTransformData().Position = parentPrefab->PositionOverride;
        entity->GetTransformData().Scale = parentPrefab->ScaleOverride;
        
        if (parentPrefab->IsRenderable || parentPrefab->IsCollidable) {
            Rect* shapeData = entity->AddShapeData();
            *shapeData = parentPrefab->ShapeOverride;
        }
        if (parentPrefab->IsRenderable) {
            RenderData* renderData = entity->AddRenderData(parentPrefab->TintOverride);
            renderData->Sprite = parentPrefab->TextureOverride;
        }
        if (parentPrefab->IsCollidable) {
            PhysicsData* physicsData = entity->AddPhysicsData(parentPrefab->IsStatic);
        }
        
        for (size_t i = 0; i < parentPrefab->GetComponentPrefabsCount(); i++) {
            parentPrefab->GetComponentPrefab(i).lock()->CreateComponent(entity.get());
        }
        
        //TODO: do the same for all children recursively
    }
    
    void Scene::MoveRefsFromPrefabToEntity(std::shared_ptr<Entity>& entity, std::unique_ptr<TemplateEntity>& prefab) {
        std::shared_ptr<const PrefabEntity> parentPrefab = m_Engine->GetEntityPrefab(prefab->GetPrefabID()).lock();
        
        for (size_t i = 0; i < parentPrefab->GetComponentPrefabsCount(); i++) {
            uint64_t compPrefabEntityID = parentPrefab->GetComponentPrefab(i).lock()->GetEntityPrefabID();
            auto compPrefabEntity = m_Engine->GetEntityPrefab(compPrefabEntityID);
        }
    }
    
    void Scene::DestroyEntity(uint64_t entityID) {
        auto it = m_RuntimeEntities.find(entityID);
        
        if (it == m_RuntimeEntities.end())
            return;
        
        size_t childCount = it->second->GetChildCount();
        
        for (size_t i = 0; i < childCount; i++) {
            if (auto child = it->second->GetChild(i).lock())
                DestroyEntity(child->GetEntityID());
        }
            
        m_RuntimeEntities.erase(it);
    }
    
    void Scene::CreateHierarchyEntity(uint64_t entityPrefabID) {
        m_HierarchyEntities.emplace_back(TemplateEntity::MakeUnique(shared_from_this(), entityPrefabID, m_HierarchyEntities.size()));
        // TODO: return the prefab overridable of the hierarchy entity
    }
    
    void Scene::Load() {
        std::unordered_map<uint64_t, uint64_t> prefabToRuntimeEntity;
        
        for (int i = 0; i < m_HierarchyEntities.size(); i++) {
            auto entity = CreateEntity().lock();
            
            m_HierarchyEntities[i]->m_RuntimeEntity = entity;
            prefabToRuntimeEntity.insert(std::make_pair(m_HierarchyEntities[i]->GetInstanceID(), entity->GetEntityID()));
            
            BuildEntityFromPrefab(entity, m_HierarchyEntities[i]);
        }
        
        for (int i = 0; i < m_HierarchyEntities.size(); i++) {
            uint64_t entityID = prefabToRuntimeEntity[m_HierarchyEntities[i]->GetInstanceID()];
            
            MoveRefsFromPrefabToEntity(m_RuntimeEntities[entityID], m_HierarchyEntities[i]);
        }
        
        m_IsLoaded = true;
    }
    
    void Scene::Unload() {
        m_RuntimeEntities.clear();
        m_IsLoaded = false;
    }
}
