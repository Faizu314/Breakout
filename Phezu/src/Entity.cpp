#include "scene/Entity.hpp"
#include "scene/Scene.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"

namespace Phezu {
    
    uint64_t Entity::s_EntitiesCount = 0;
    
    Entity::Entity(const std::weak_ptr<Scene> scene) : m_Scene(scene), m_TransformData(this) {
        m_EntityID = s_EntitiesCount;
        s_EntitiesCount++;
        
        m_ShapeData = nullptr;
        m_RenderData = nullptr;
        m_PhysicsData = nullptr;
        m_Parent = nullptr;
        m_IsActive = true;
    }
    
    Entity::~Entity() {
        if (m_PhysicsData != nullptr)
            delete m_PhysicsData;
        if (m_ShapeData != nullptr)
            delete m_ShapeData;
        if (m_RenderData != nullptr)
            delete m_RenderData;
        
        auto scene = m_Scene.lock();
        if (!scene)
            return;
        
        for (int i = 0; i < m_Children.size(); i++) {
            if (auto child = m_Children[i].lock())
                scene->DestroyEntity(child->GetEntityID());
        }
    }
    
    uint64_t Entity::GetEntityID() const {
        return m_EntityID;
    }
    
    
    void Entity::SetActive(bool isActive) {
        m_IsActive = isActive;
    }
    bool Entity::GetActive() const {
        return m_IsActive;
    }
    TransformData& Entity::GetTransformData() {
        return m_TransformData;
    }
    ShapeData* const Entity::AddShapeData() {
        m_ShapeData = new ShapeData(this);
        return m_ShapeData;
    }
    RenderData* const Entity::AddRenderData(Color tint) {
        m_RenderData = new RenderData(this, tint);
        return m_RenderData;
    }
    PhysicsData* const Entity::AddPhysicsData(bool isStatic) {
        m_PhysicsData = new PhysicsData(this, isStatic);
        return m_PhysicsData;
    }
    TransformData* const Entity::GetParent() {
        return m_Parent;
    }
    size_t Entity::GetChildCount() {
        return m_Children.size();
    }
    std::weak_ptr<Entity> Entity::GetChild(size_t childIndex) {
        if (childIndex >= m_Children.size())
            return std::weak_ptr<Entity>();
        
        return m_Children[childIndex];
    }
    
    void Entity::AddChild(std::weak_ptr<Entity> child) {
        m_Children.push_back(child);
    }
    
    void Entity::RecalculateTransformations() {
        m_TransformData.RecalculateTransformations();
    }
    
    void Entity::RecalculateSubtreeTransformations() {
        for (auto child_ : m_Children) {
            auto child = child_.lock();
            child->RecalculateTransformations();
            child->RecalculateSubtreeTransformations();
        }
    }
    
    void Entity::SetParent(std::weak_ptr<Entity> parent) {
        SetParent_Internal(m_Scene.lock()->GetEntity(m_EntityID), parent);
    }
    
    void SetParent_Internal(std::weak_ptr<Entity> _this, std::weak_ptr<Entity> parent) {
        auto _thisLocked = _this.lock();
        if (auto p = parent.lock()) {
            _thisLocked->m_Parent = &p->GetTransformData();
            p->AddChild(_this);
            _thisLocked->RecalculateTransformations();
            _thisLocked->RecalculateSubtreeTransformations();
        }
    }
    
    void Entity::RemoveParent() {
        
    }
    
    void Entity::OnChildDestroyed() {
        for (int i = 0; i < m_Children.size(); i++) {
            if (m_Children[i].expired()) {
                m_Children.erase(m_Children.begin() + i);
                return;
            }
        }
    }
    
}
