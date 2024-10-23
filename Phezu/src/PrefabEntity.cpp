#include "scene/PrefabEntity.hpp"
#include "scene/components/BehaviourComponentPrefab.hpp"

namespace Phezu {
    
    PrefabEntity::PrefabEntity()
    : m_PrefabEntityID(s_EntityPrefabsCount), m_IsRoot(true), m_Root(0), m_PathSize(0) {
        s_EntityPrefabsCount++;
    }
    
    PrefabEntity::PrefabEntity(uint64_t root, std::unique_ptr<size_t[]> path, size_t pathSize)
    : m_PrefabEntityID(0), m_IsRoot(false), m_Root(root), m_Path(std::move(path)), m_PathSize(pathSize) {
        
    }
    
    PrefabEntity::~PrefabEntity() {
        for (PrefabEntity* child : m_Children)
            delete child;
    }
    
    uint64_t PrefabEntity::GetPrefabEntityID() const { return m_PrefabEntityID; }
    
    PrefabEntity& PrefabEntity::CreateChildEntity() {
        uint64_t root = m_IsRoot ? m_PrefabEntityID : m_Root;
        std::unique_ptr<size_t[]> path = std::make_unique<size_t[]>(m_PathSize + 1);
        for (int i = 0; i < m_PathSize; i++)
            path[i] = m_Path[i];
        path[m_PathSize] = m_Children.size();
        
        PrefabEntity* child = new PrefabEntity(root, std::move(path), m_PathSize + 1);
        m_Children.push_back(child);
        
        return *child;
    }
    
    template<typename T>
    std::weak_ptr<T> PrefabEntity::AddComponentPrefab() {
        if (!std::is_base_of<BehaviourComponentPrefab, T>::value) {
            //TODO: copy and paste the logging class
            return;
        }
        
        uint8_t componentID = m_BehaviourComponents.size(); //TODO: This should be the count of other BehaviourComponentPrefab that are of the same type as T
        
        m_BehaviourComponents.emplace_back(std::make_shared<T>(m_PrefabEntityID, componentID));
        return m_BehaviourComponents[m_BehaviourComponents.size() - 1];
    }
}
