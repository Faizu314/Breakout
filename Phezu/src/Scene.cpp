#include "scene/Scene.hpp"
#include "scene/Entity.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"
#include "scene/Prefab.hpp"
#include "scene/components/BehaviourComponentPrefab.hpp"
#include "scene/EntityTemplate.hpp"
#include "Engine.hpp"

#include <stdexcept>

namespace Phezu {
    
    Scene::Scene(Engine* engine, const std::string& name) : m_Engine(engine), m_Name(name), m_IsLoaded(false), m_IsSceneToRuntimeMappingValid(false) { }
    
    std::weak_ptr<Entity> Scene::CreateEntity() {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(shared_from_this());
        m_RuntimeEntities.insert(std::make_pair(entity->GetEntityID(), entity));
        
        return entity;
    }
    
    std::weak_ptr<Entity> Scene::CreateEntity(uint64_t prefabID) {
        auto prefab = m_Engine->GetPrefab(prefabID).lock();
        
        if (!prefab) {
            //TODO: logging
            return std::weak_ptr<Entity>();
        }
        
        auto entity = CreateEntity().lock();
        BuildEntityFromPrefabEntity(entity, &prefab->RootEntity);
        ApplyPrefabOverridesToEntity(entity, &prefab->RootEntity);
        
        CallStartOnEntity(entity);
        
        return entity;
    }
    
    void Scene::CallStartOnEntity(std::shared_ptr<Entity> entity) {
        for (auto comp : entity->m_BehaviourComponents) {
            comp->Start();
        }
        
        for (auto child : entity->m_Children)
            CallStartOnEntity(child.lock());
    }
    
    std::weak_ptr<Entity> Scene::GetEntity(uint64_t entityID) const {
        try {
            return m_RuntimeEntities.at(entityID);
        }
        catch (const std::out_of_range&) {
            return std::weak_ptr<Entity>();
        }
    }
    
    void Scene::DestroyEntity(uint64_t entityID) {
        if (std::find(m_EntitiesToDestroy.begin(), m_EntitiesToDestroy.end(), entityID) != m_EntitiesToDestroy.end())
            return;
        
        m_EntitiesToDestroy.push_back(entityID);
    }
    
    void Scene::DestroyEntityInternal(uint64_t entityID) {
        auto it = m_RuntimeEntities.find(entityID);
        
        if (it == m_RuntimeEntities.end())
            return;
        
        size_t childCount = it->second->GetChildCount();
        
        for (size_t i = 0; i < childCount; i++) {
            if (auto child = it->second->GetChild(i).lock())
                DestroyEntityInternal(child->GetEntityID());
        }
        
        for (auto comp : it->second->m_BehaviourComponents)
            comp->OnDestroy();
        it->second->OnDestroyed();
        
        m_RuntimeEntities.erase(it);
    }
    
    void Scene::CreateSceneEntity(uint64_t prefabEntityID, Vector2 positionOverride, std::string tag) {
        m_SceneEntities.push_back(EntityTemplate::MakeUnique(shared_from_this(), prefabEntityID));
        m_SceneEntities[m_SceneEntities.size() - 1]->OverrideTag = tag.compare("Default") != 0;
        m_SceneEntities[m_SceneEntities.size() - 1]->TagOverride = tag;
        m_SceneEntities[m_SceneEntities.size() - 1]->OverridePosition = true;
        m_SceneEntities[m_SceneEntities.size() - 1]->PositionOverride = positionOverride;
    }
    
    void Scene::BuildEntityFromTemplate(std::shared_ptr<Entity> entity, std::unique_ptr<EntityTemplate>& entityTemplate) {
        std::shared_ptr<const Prefab> prefab = m_Engine->GetPrefab(entityTemplate->GetPrefabID()).lock();
        
        BuildEntityFromPrefabEntity(entity, &prefab->RootEntity);
        
        //TODO: Apply all template overrides here
        if (entityTemplate->OverridePosition)
            entity->GetTransformData()->SetLocalPosition(entityTemplate->PositionOverride);
        if (entityTemplate->OverrideTag)
            entity->SetTag(entityTemplate->TagOverride);
    }
    
    void Scene::BuildEntityFromPrefabEntity(std::shared_ptr<Entity> entity, const PrefabEntity* prefabEntity) {
        entity->SetTag(prefabEntity->TagOverride);
        
        entity->GetTransformData()->SetLocalPosition(prefabEntity->PositionOverride);
        entity->GetTransformData()->SetLocalScale(prefabEntity->ScaleOverride);
        
        if (prefabEntity->IsRenderable || prefabEntity->IsCollidable) {
            ShapeData* shapeData = entity->AddShapeData();
            shapeData->SetPivot(prefabEntity->ShapePivotOverride);
            shapeData->SetSize(prefabEntity->ShapeSizeOverride);
        }
        if (prefabEntity->IsRenderable) {
            RenderData* renderData = entity->AddRenderData(prefabEntity->TintOverride);
            renderData->Sprite = prefabEntity->TextureOverride;
            renderData->RectUVs = prefabEntity->UVsOverride;
        }
        if (prefabEntity->IsCollidable) {
            std::weak_ptr<PhysicsData> physicsData = entity->AddPhysicsData(prefabEntity->IsStatic);
        }
        
        for (size_t i = 0; i < prefabEntity->GetComponentPrefabsCount(); i++) {
            prefabEntity->GetComponentPrefab(i).lock()->CreateComponent(entity);
        }
        
        for (size_t i = 0; i < prefabEntity->GetChildCount(); i++) {
            std::shared_ptr<Entity> child = CreateEntity().lock();
            child->SetParent(entity);
            
            BuildEntityFromPrefabEntity(child, prefabEntity->GetChild(i));
        }
    }
    
    void Scene::ApplyTemplateOverridesToEntity(std::shared_ptr<Entity> entity, std::unique_ptr<EntityTemplate>& entityTemplate) {
        std::shared_ptr<const Prefab> prefab = m_Engine->GetPrefab(entityTemplate->GetPrefabID()).lock();
        
        ApplyPrefabOverridesToEntity(entity, &prefab->RootEntity);
        
    }
    
    void Scene::ApplyPrefabOverridesToEntity(std::shared_ptr<Entity> entity, const PrefabEntity* prefabEntity) {
        for (size_t i = 0; i < prefabEntity->GetComponentPrefabsCount(); i++) {
            auto componentPrefab = prefabEntity->GetComponentPrefab(i).lock();
            auto runtimeComponent = componentPrefab->GetRuntimeComponent(shared_from_this(), entity).lock();
            
            if (!runtimeComponent) {
                //TODO: logging
                continue;
            }
            
            componentPrefab->InitRuntimeComponentInternal(shared_from_this(), runtimeComponent);
        }
        
        for (size_t i = 0; i < prefabEntity->GetChildCount(); i++) {
            ApplyPrefabOverridesToEntity(entity->GetChild(i).lock(), prefabEntity->GetChild(i));
        }
    }
    
    void Scene::Load() {
        for (size_t i = 0; i < m_SceneEntities.size(); i++) {
            auto entity = CreateEntity().lock();
            
            m_SceneEntities[i]->m_RuntimeRootEntity = entity;
            
            BuildEntityFromTemplate(entity, m_SceneEntities[i]);
        }
        
        m_IsSceneToRuntimeMappingValid = true;
        
        for (size_t i = 0; i < m_SceneEntities.size(); i++) {
            ApplyTemplateOverridesToEntity(m_SceneEntities[i]->m_RuntimeRootEntity.lock(), m_SceneEntities[i]);
        }
        
        m_IsLoaded = true;
        
        for (auto [id, entity] : m_RuntimeEntities) {
            if (entity->IsDirty()) {
                entity->RecalculateSubtreeTransformations();
            }
        }
        
        for (auto [id, entity] : m_RuntimeEntities)
            CallStartOnEntity(entity);
    }
    
    void Scene::LogicUpdate(float deltaTime) {
        for (auto entityID : m_EntitiesToDestroy)
            DestroyEntityInternal(entityID);
        
        m_EntitiesToDestroy.clear();
        
        for (auto entity : m_RuntimeEntities) {
            for (auto comp : entity.second->GetComponents<BehaviourComponent>()) {
                comp.lock()->Update(deltaTime);
                
                if (!m_IsLoaded)
                    return;
            }
        }
        
        UpdateHierarchy();
    }
    
    void Scene::UpdateHierarchy() {
        for (auto entity : m_RuntimeEntities) {
            if (entity.second->IsDirty()) {
                entity.second->RecalculateSubtreeTransformations();
            }
        }
    }
    
    void Scene::GetPhysicsEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& staticCount, size_t& dynamicCount) const {
        staticCount = dynamicCount = 0;
        
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            auto physicsData = entity->GetPhysicsData().lock();
            if (physicsData && physicsData->IsStatic() && entity->GetShapeData() != nullptr) {
                if (staticCount < entities.size())
                    entities[staticCount] = entity;
                else
                    entities.push_back(entity);
                staticCount++;
            }
        }
        
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            auto physicsData = entity->GetPhysicsData().lock();
            if (physicsData && !physicsData->IsStatic() && entity->GetShapeData() != nullptr) {
                if (staticCount + dynamicCount < entities.size())
                    entities[staticCount + dynamicCount] = entity;
                else
                    entities.push_back(entity);
                dynamicCount++;
            }
        }
    }
    
    void Scene::GetRenderableEntities(std::vector<std::weak_ptr<Entity>>& entities, size_t& count) const {
        count = 0;
        for (auto it = m_RuntimeEntities.begin(); it != m_RuntimeEntities.end(); it++) {
            auto entity = (*it).second;
            if (entity->GetRenderData() != nullptr && entity->GetShapeData() != nullptr) {
                if (count < entities.size())
                    entities[count] = entity;
                else
                    entities.push_back(entity);
                count++;
            }
        }
    }
    
    void Scene::BeginUnload() {
        m_IsLoaded = false;
        m_IsSceneToRuntimeMappingValid = false;
    }
    
    void Scene::Unload() {
        m_RuntimeEntities.clear();
        m_IsLoaded = false;
        m_IsSceneToRuntimeMappingValid = false;
    }
    
    long long unsigned int Scene::GetFrameCount() const {
        return m_Engine->GetFrameCount();
    }
}
