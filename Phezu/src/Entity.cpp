#include "scene/Entity.hpp"
#include "scene/Scene.hpp"
#include "scene/components/RenderData.hpp"
#include "scene/components/PhysicsData.hpp"
#include "scene/components/BehaviourComponent.hpp"

namespace Phezu {
    
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
    Rect* const Entity::AddShapeData() {
        m_ShapeData = new Rect();
        memset(m_ShapeData, 0, sizeof(Rect));
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
    
    void Entity::SetParent(std::weak_ptr<Entity> parent) {
        SetParent_Internal(m_Scene.lock()->GetEntity(m_EntityID), parent);
    }
    
    void SetParent_Internal(std::weak_ptr<Entity> _this, std::weak_ptr<Entity> parent) {
        auto _thisLocked = _this.lock();
        if (auto p = parent.lock()) {
            _thisLocked->m_Parent = &p->GetTransformData();
            p->AddChild(_this);
        }
    }
    
    void Entity::RemoveParent() {
        
    }
    
    
    template<typename T>
    std::weak_ptr<T> Entity::GetComponent() {
        if (!std::is_base_of<BehaviourComponent, T>::value) {
            //TODO: copy and paste the logging class
            return;
        }
        
        for (int i = 0; i < m_BehaviourComponents.size(); i++) {
            auto* componentPtr = m_BehaviourComponents[i].get();
            if (typeid(*componentPtr) == typeid(T)) {
                return m_BehaviourComponents[i];
            }
        }
    }
    
    template<typename T>
    std::weak_ptr<T> Entity::AddComponent() {
        static_assert(!std::is_base_of<BehaviourComponent, T>::value, "Component T is not of type BehaviourComponent");
        
        uint8_t componentID = m_BehaviourComponents.size(); //TODO: This should be the count of other BehaviourComponent that are of the same type as T
        
        return m_BehaviourComponents.emplace_back(std::make_shared<T>(this), componentID);
    }
    
    template<typename T>
    void Entity::RemoveComponent() {
        if (!std::is_base_of<BehaviourComponent, T>::value) {
            //TODO: copy and paste the logging class
            return;
        }
        
        for (int i = 0; i < m_BehaviourComponents.size(); i++) {
            auto* componentPtr = m_BehaviourComponents[i].get();
            if (typeid(*componentPtr) == typeid(T)) {
                m_BehaviourComponents.erase(m_BehaviourComponents.begin() + i);
                return;
            }
        }
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
