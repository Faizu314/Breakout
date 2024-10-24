#include "scene/EntityTemplate.hpp"

namespace Phezu {
    
    EntityTemplate::EntityTemplate(ConstructionToken token, std::weak_ptr<Scene> scene, uint64_t prefabID, uint64_t instanceID)
    : m_Scene(scene), m_PrefabID(prefabID), m_InstanceID(instanceID), OverridePosition(false) {
        
    }
    
    uint64_t EntityTemplate::GetPrefabID() const { return m_PrefabID; }
    uint64_t EntityTemplate::GetInstanceID() const { return m_InstanceID; }
    
    std::shared_ptr<EntityTemplate> EntityTemplate::MakeShared(std::weak_ptr<Scene> scene, uint64_t prefabID, uint64_t instanceID) {
        ConstructionToken token;
        return std::make_shared<EntityTemplate>(token, scene, prefabID, instanceID);
    }
    
    std::unique_ptr<EntityTemplate> EntityTemplate::MakeUnique(std::weak_ptr<Scene> scene, uint64_t prefabID, uint64_t instanceID) {
        ConstructionToken token;
        return std::make_unique<EntityTemplate>(token, scene, prefabID, instanceID);
    }
    
    std::weak_ptr<Entity> GetRuntimeEntity(PrefabEntity& entityPrefab) {
        //check if root prefab of componentPrefab is the same as the parent prefab of hierarchy entity.
        //get path from the root to it.
        //get runtime entity of hierarchy entity, traverse to the child entity using the path.
    }
    
    std::weak_ptr<BehaviourComponent> GetRuntimeBehaviourComponent(BehaviourComponentPrefab& componentPrefab) {
        //check if root prefab of componentPrefab is the same as the parent prefab of hierarchy entity.
        //get path from the root prefab to its entity in prefab.
        //get runtime entity of hierarchy entity, traverse to the child entity using the path and get the component of the same type and id as componentPrefab.
    }
}