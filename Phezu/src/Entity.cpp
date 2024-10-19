#include "scene/Entity.hpp"

namespace Phezu {
    
    Entity::Entity(const std::weak_ptr<Scene> scene) : m_Scene(scene), m_TransformData(this), m_RenderData(this) {
        m_EntityID = s_EntitiesCount;
        s_EntitiesCount++;
        
        memset(&m_ShapeData, 0, sizeof(m_ShapeData));
        m_PhysicsData = nullptr;
        m_Parent = nullptr;
        m_IsActive = true;
    }
    
    Entity::~Entity() {
        if (m_PhysicsData != nullptr)
            delete m_PhysicsData;
        
        auto scene = m_Scene.lock();
        if (!scene)
            return;
        
        for (int i = 0; i < m_Children.size(); i++) {
            scene->DestroyEntity(m_Children[i]);
        }
    }
    
    uint64_t Entity::GetID() {
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
    Rect& Entity::GetShapeData() {
        return m_ShapeData;
    }
    RenderData& Entity::GetRenderData() {
        return m_RenderData;
    }
    PhysicsData& Entity::GetPhysicsData() {
        return *m_PhysicsData;
    }
    TransformData& Entity::GetParent() {
        return *m_Parent;
    }
    size_t Entity::GetChildCount() {
        return m_Children.size();
    }
    std::weak_ptr<Entity> Entity::GetChild(size_t childIndex) {
        if (childIndex >= m_Children.size())
            return std::weak_ptr<Entity>();
        
        return m_Children[childIndex];
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
        
        return m_BehaviourComponents.emplace_back(std::make_shared<T>(this));
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
