#include "scene/components/BehaviourComponentPrefab.hpp"

namespace Phezu {
    
    BehaviourComponentPrefab::BehaviourComponentPrefab(uint64_t entityPrefab, uint8_t componentID)
    : m_EntityPrefab(entityPrefab), m_ComponentID(componentID) { }
    
    uint64_t BehaviourComponentPrefab::GetEntityPrefabID() { return m_EntityPrefab; }
    uint8_t BehaviourComponentPrefab::GetComponentID() { return m_ComponentID; }
}
