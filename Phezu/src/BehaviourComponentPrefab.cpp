#include "scene/components/BehaviourComponentPrefab.hpp"
#include "Phezu.hpp"
#include "scene/Entity.hpp"
#include "scene/Scene.hpp"

namespace Phezu {
    
    BehaviourComponentPrefabBase::BehaviourComponentPrefabBase(uint64_t entityPrefab, std::unique_ptr<size_t[]> pathToPrefabEntity, size_t pathSize, uint8_t componentID)
    : m_PrefabID(entityPrefab), m_PathToPrefabEntity(std::move(pathToPrefabEntity)), m_PathSize(pathSize), m_ComponentID(componentID) { }
    
    uint64_t BehaviourComponentPrefabBase::GetEntityPrefabID() const { return m_PrefabID; }
    uint8_t BehaviourComponentPrefabBase::GetComponentID() const { return m_ComponentID; }

}
