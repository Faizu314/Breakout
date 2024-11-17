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
}
