#include "scene/EntityPrefab.hpp"
#include "scene/components/BehaviourComponentPrefab.hpp"

namespace Phezu {
    
    EntityPrefab::EntityPrefab()
    : m_PrefabID(s_EntityPrefabsCount), m_IsRoot(true), m_Root(0), m_PathSize(0) {
        s_EntityPrefabsCount++;
    }
    
    EntityPrefab::EntityPrefab(uint64_t root, std::unique_ptr<size_t[]> path, size_t pathSize)
    : m_PrefabID(0), m_IsRoot(false), m_Root(root), m_Path(std::move(path)), m_PathSize(pathSize) {
        
    }
    
    EntityPrefab::~EntityPrefab() {
        for (EntityPrefab* child : m_Children)
            delete child;
    }
    
    uint64_t EntityPrefab::GetID() const { return m_PrefabID; }
    
    EntityPrefab& EntityPrefab::CreateChildEntity() {
        uint64_t root = m_IsRoot ? m_PrefabID : m_Root;
        std::unique_ptr<size_t[]> path = std::make_unique<size_t[]>(m_PathSize + 1);
        for (int i = 0; i < m_PathSize; i++)
            path[i] = m_Path[i];
        path[m_PathSize] = m_Children.size();
        
        EntityPrefab* child = new EntityPrefab(root, std::move(path), m_PathSize + 1);
        m_Children.push_back(child);
        
        return *child;
    }
    
    template<typename T>
    std::weak_ptr<T> EntityPrefab::AddComponentPrefab() {
        if (!std::is_base_of<BehaviourComponentPrefab, T>::value) {
            //TODO: copy and paste the logging class
            return;
        }
        
        uint8_t componentID = m_BehaviourComponents.size(); //TODO: This should be the count of other BehaviourComponentPrefab that are of the same type as T
        
        m_BehaviourComponents.emplace_back(std::make_shared<T>(m_PrefabID, componentID));
        return m_BehaviourComponents[m_BehaviourComponents.size() - 1];
    }
}
