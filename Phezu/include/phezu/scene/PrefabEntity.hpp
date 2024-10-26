#pragma once

#include <vector>
#include "Renderer.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class Entity;
    class BehaviourComponentPrefabBase;
    
    class PrefabEntity {
    public:
        PrefabEntity();
        ~PrefabEntity();
        uint64_t GetPrefabEntityID() const;
    public:
        Vector2 PositionOverride;
        Vector2 ScaleOverride;
        bool IsRenderable;
        bool IsCollidable;
        bool IsStatic;
        Rect ShapeOverride;
        std::weak_ptr<Texture> TextureOverride;
        Color TintOverride;
    public:
        template<typename T>
        std::weak_ptr<T> AddComponentPrefab() {
            if (!std::is_base_of<BehaviourComponentPrefabBase, T>::value) {
                //TODO: copy and paste the logging class
                return;
            }
            
            uint8_t componentID = m_BehaviourComponents.size(); //TODO: This should be the count of other BehaviourComponentPrefab that are of the same type as T
            
            std::unique_ptr<size_t[]> path = std::make_unique<size_t[]>(m_PathSize);
            for (int i = 0; i < m_PathSize; i++)
                path[i] = m_Path[i];
            
            std::shared_ptr<T> component = std::make_shared<T>(m_PrefabEntityID, std::move(path), m_PathSize, componentID);
            
            m_BehaviourComponents.push_back(std::static_pointer_cast<BehaviourComponentPrefabBase>(component));
            
            return component;
        }
    public:
        PrefabEntity& CreateChildEntity();
        size_t GetComponentPrefabsCount() const;
        std::weak_ptr<BehaviourComponentPrefabBase> GetComponentPrefab(size_t index) const;
        size_t GetChildCount() const;
        const PrefabEntity* GetChild(size_t childIndex) const;
        std::weak_ptr<Entity> GetRuntimeEntity(uint64_t instanceID);
    private:
        PrefabEntity(uint64_t root, std::unique_ptr<size_t[]> path, size_t pathSize);
    private:
        static uint64_t s_PrefabEntityCount;
        const uint64_t m_PrefabEntityID;
        const bool m_IsRoot;
        const uint64_t m_Root;
        const std::unique_ptr<size_t[]> m_Path;
        const size_t m_PathSize;
    private:
        std::vector<std::shared_ptr<BehaviourComponentPrefabBase>> m_BehaviourComponents;
        std::vector<PrefabEntity*> m_Children;
    };
}
