#include "scene/components/BehaviourComponentPrefab.hpp"
#include "Phezu.hpp"
#include "scene/Entity.hpp"
#include "scene/Scene.hpp"

namespace Phezu {
    
    BehaviourComponentPrefabBase::BehaviourComponentPrefabBase(uint64_t entityPrefab, std::unique_ptr<size_t[]> pathToPrefabEntity, size_t pathSize, uint8_t componentID)
    : m_PrefabID(entityPrefab), m_PathToPrefabEntity(std::move(pathToPrefabEntity)), m_PathSize(pathSize), m_ComponentID(componentID) { }
    
    uint64_t BehaviourComponentPrefabBase::GetEntityPrefabID() const { return m_PrefabID; }
    uint8_t BehaviourComponentPrefabBase::GetComponentID() const { return m_ComponentID; }
    
    template <typename T>
    std::weak_ptr<BehaviourComponent> BehaviourComponentPrefab<T>::CreateComponent(std::weak_ptr<Entity> entity) const {
        entity.lock()->AddComponent<T>();
    }
    
    template <typename T>
    std::weak_ptr<Entity> BehaviourComponentPrefab<T>::GetRuntimeEntity(std::weak_ptr<Scene> scene, uint64_t instanceID) {
        auto sceneL = scene.lock();
        std::weak_ptr<const Prefab> prefab = sceneL->GetPrefab(m_PrefabID);
        std::shared_ptr<Entity> rootEntity = sceneL->GetRuntimeEntityFromSceneEntity(instanceID).lock();
        
        std::shared_ptr<Entity> targetEntity = rootEntity;
        for (size_t i = 0; i < m_PathSize; i++) {
            targetEntity = targetEntity->GetChild(m_PathToPrefabEntity[i]).lock();
        }
        
        return targetEntity;
    }
    
    template <typename T>
    std::weak_ptr<T> BehaviourComponentPrefab<T>::GetRuntimeComponent(std::weak_ptr<Scene> scene, uint64_t instanceID) {
        std::weak_ptr<Entity> targetEntity = GetRuntimeEntity(scene, instanceID);
        std::shared_ptr<Entity> targetEntityL = targetEntity.lock();
        
        //TODO: Get the right component if there are multiple components of the same type attached
        return targetEntityL->GetComponent<T>();
    }
    
    template <typename T>
    bool BehaviourComponentPrefab<T>::IsOfType(std::type_index type) const {
        return std::type_index(typeid(T)) == type;
    }
}
