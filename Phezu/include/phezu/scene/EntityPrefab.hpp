#pragma once

#include <vector>
#include "Renderer.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class BehaviourComponentPrefab;
    
    class EntityPrefab {
    public:
        EntityPrefab();
        ~EntityPrefab();
        uint64_t GetID() const;
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
        EntityPrefab& CreateChildEntity();
        template<typename T>
        std::weak_ptr<T> AddComponentPrefab();
        size_t GetComponentPrefabsCount() const;
        std::weak_ptr<BehaviourComponentPrefab> GetComponentPrefab(size_t index) const;
    private:
        EntityPrefab(uint64_t root, std::unique_ptr<size_t[]> path, size_t pathSize);
    private:
        static uint64_t s_EntityPrefabsCount;
        const uint64_t m_PrefabID;
        const bool m_IsRoot;
        const uint64_t m_Root;
        const std::unique_ptr<size_t[]> m_Path;
        const size_t m_PathSize;
    private:
        std::vector<std::shared_ptr<BehaviourComponentPrefab>> m_BehaviourComponents;
        std::vector<EntityPrefab*> m_Children;
    };
    
    uint64_t EntityPrefab::s_EntityPrefabsCount = 0;
    
    EntityPrefab DEFAULT_PREFAB;
}
