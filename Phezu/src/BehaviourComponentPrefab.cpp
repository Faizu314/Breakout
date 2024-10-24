#include "scene/components/BehaviourComponentPrefab.hpp"
#include "Phezu.hpp"
#include "scene/Scene.hpp"

namespace Phezu {
    
    BehaviourComponentPrefab::BehaviourComponentPrefab(uint64_t entityPrefab, std::unique_ptr<size_t[]> pathToPrefabEntity, uint8_t componentID)
    : m_PrefabID(entityPrefab), m_PathToPrefabEntity(std::move(pathToPrefabEntity)), m_ComponentID(componentID) { }
    
    uint64_t BehaviourComponentPrefab::GetEntityPrefabID() { return m_PrefabID; }
    uint8_t BehaviourComponentPrefab::GetComponentID() { return m_ComponentID; }
    
    std::weak_ptr<BehaviourComponent> BehaviourComponentPrefab::GetRuntimeComponent(std::weak_ptr<Scene> scene, uint64_t instanceID) {
        auto sceneL = scene.lock();
        std::weak_ptr<const Prefab> prefab = sceneL->GetPrefab(m_PrefabID);
        std::weak_ptr<Entity> rootEntity = sceneL->GetRuntimeEntityFromSceneEntity(instanceID);
        
        //for (int i = 0; i < m_PathToPrefabEntity->)
    }
}
