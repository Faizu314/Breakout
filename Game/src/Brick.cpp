#include "Brick.hpp"
#include "GameManager.hpp"

const Phezu::Color BRICK_COLORS[3] = { Phezu::Color(255, 0, 0, 255), Phezu::Color(0, 255, 0, 255), Phezu::Color(0, 0, 255, 255) };

void Brick::Start() {
    m_PhysicsData = m_Entity->GetPhysicsData().lock();
    m_PhysicsData->RegisterOnCollisionEnter(this, &Brick::OnCollisionEnter);
    
    m_RenderData = m_Entity->GetRenderData();
    m_RenderData->Tint = BRICK_COLORS[m_TotalHealth - 1];
    
    m_CurrentHealth = m_TotalHealth;
}

void Brick::OnDestroy() {
    _GameManager->OnBrickBroken();
    m_PhysicsData->UnregisterOnCollisionEnter(this);
}

void Brick::OnCollisionEnter(const Phezu::Collision& collision) {
    m_CurrentHealth--;
    
    m_RenderData->Tint = BRICK_COLORS[m_CurrentHealth - 1];
    
    if (m_CurrentHealth <= 0)
        Phezu::Destroy(m_Entity);
}
