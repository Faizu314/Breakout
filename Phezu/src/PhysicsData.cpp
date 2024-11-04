#include "scene/components/PhysicsData.hpp"

namespace Phezu {
    
    void PhysicsData::OnCollisionEnter(const Collision &collision) {
        for (const auto& [behaviour, listener] : m_OnCollisionEnterEvent) {
            listener(collision);
        }
    }
    
    void PhysicsData::OnCollisionStay(const Collision &collision) {
        for (const auto& [behaviour, listener] : m_OnCollisionStayEvent) {
            listener(collision);
        }
    }
    
    void PhysicsData::OnCollisionExit(const Collision &collision) {
        for (const auto& [behaviour, listener] : m_OnCollisionExitEvent) {
            listener(collision);
        }
    }
    
    void PhysicsData::UnregisterOnCollisionEnter(BehaviourComponent *comp) {
        if (m_OnCollisionEnterEvent.find(comp) == m_OnCollisionEnterEvent.end())
            return;
        
        m_OnCollisionEnterEvent.erase(comp);
    }
    
    void PhysicsData::UnregisterOnCollisionStay(BehaviourComponent *comp) {
        if (m_OnCollisionStayEvent.find(comp) == m_OnCollisionStayEvent.end())
            return;
        
        m_OnCollisionStayEvent.erase(comp);
    }
    
    void PhysicsData::UnregisterOnCollisionExit(BehaviourComponent *comp) {
        if (m_OnCollisionExitEvent.find(comp) == m_OnCollisionExitEvent.end())
            return;
        
        m_OnCollisionExitEvent.erase(comp);
    }
}
