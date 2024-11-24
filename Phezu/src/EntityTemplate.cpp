#include "scene/EntityTemplate.hpp"

namespace Phezu {
    
    EntityTemplate::EntityTemplate(ConstructionToken token, std::weak_ptr<Scene> scene, uint64_t prefabID)
    : m_Scene(scene), m_PrefabID(prefabID), OverridePosition(false) {
        
    }
    
    uint64_t EntityTemplate::GetPrefabID() const { return m_PrefabID; }
    
    std::shared_ptr<EntityTemplate> EntityTemplate::MakeShared(std::weak_ptr<Scene> scene, uint64_t prefabID) {
        ConstructionToken token;
        return std::make_shared<EntityTemplate>(token, scene, prefabID);
    }
    
    std::unique_ptr<EntityTemplate> EntityTemplate::MakeUnique(std::weak_ptr<Scene> scene, uint64_t prefabID) {
        ConstructionToken token;
        return std::make_unique<EntityTemplate>(token, scene, prefabID);
    }
}
