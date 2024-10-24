#pragma once

#include <vector>
#include "Renderer.hpp"
#include "maths/Math.hpp"

namespace Phezu {
    
    class Entity;
    class BehaviourComponentPrefab;
    
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
        PrefabEntity& CreateChildEntity();
        template<typename T>
        std::weak_ptr<T> AddComponentPrefab();
        size_t GetComponentPrefabsCount() const;
        std::weak_ptr<BehaviourComponentPrefab> GetComponentPrefab(size_t index) const;
        size_t GetChildCount() const;
        const PrefabEntity* GetChild(size_t childIndex) const;
        std::weak_ptr<Entity> GetRuntimeEntity(uint64_t instanceID);
    private:
        PrefabEntity(uint64_t root, std::unique_ptr<size_t[]> path, size_t pathSize);
    private:
        static uint64_t s_EntityPrefabsCount;
        const uint64_t m_PrefabEntityID;
        const bool m_IsRoot;
        const uint64_t m_Root;
        const std::unique_ptr<size_t[]> m_Path;
        const size_t m_PathSize;
    private:
        std::vector<std::shared_ptr<BehaviourComponentPrefab>> m_BehaviourComponents;
        std::vector<PrefabEntity*> m_Children;
    };
    
    uint64_t PrefabEntity::s_EntityPrefabsCount = 0;
    
    PrefabEntity DEFAULT_PREFAB;
}
