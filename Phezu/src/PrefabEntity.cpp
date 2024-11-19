#include "scene/PrefabEntity.hpp"

namespace Phezu {
    
    uint64_t PrefabEntity::s_PrefabEntityCount = 0;
    PrefabEntity DEFAULT_PREFAB;
    
    PrefabEntity::PrefabEntity()
    : m_PrefabEntityID(s_PrefabEntityCount), m_IsRoot(true), m_Root(0), m_PathSize(0), ScaleOverride(Vector2(1, 1)), ShapeSizeOverride(Vector2(10, 10)), TintOverride(Color::White) {
        s_PrefabEntityCount++;
    }
    
    PrefabEntity::PrefabEntity(uint64_t root, std::unique_ptr<size_t[]> path, size_t pathSize)
    : m_PrefabEntityID(0), m_IsRoot(false), m_Root(root), m_Path(std::move(path)), m_PathSize(pathSize), ScaleOverride(Vector2(1, 1)), ShapeSizeOverride(Vector2(10, 10)), TintOverride(Color::White) {}
    
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
    
    size_t PrefabEntity::GetChildCount() const {
        return m_Children.size();
    }
    
    const PrefabEntity* PrefabEntity::GetChild(size_t childIndex) const {
        if (childIndex >= m_Children.size())
            return nullptr;
        
        return m_Children[childIndex];
    }
    
    std::weak_ptr<BehaviourComponentPrefabBase> PrefabEntity::GetComponentPrefab(size_t index) const {
        if (index >= m_BehaviourComponents.size()) {
            //TODO: copy and paste the logging class
            return std::weak_ptr<BehaviourComponentPrefabBase>();
        }
        return m_BehaviourComponents[index];
    }
    
    size_t PrefabEntity::GetComponentPrefabsCount() const {
        return m_BehaviourComponents.size();
    }
}
