#include "Game.hpp"
#include "Ball.hpp"

void Ball::Start() {
    m_RenderData = m_Entity->GetRenderData();
    
    auto physicsData = m_Entity->GetPhysicsData().lock();
    physicsData->Velocity = m_StartVelocity;
    
    physicsData->RegisterOnCollisionEnter(this, &Ball::OnCollisionEnter);
    physicsData->RegisterOnCollisionExit(this, &Ball::OnCollisionExit);
}

void Ball::OnCollisionEnter(const Phezu::Collision& collision) {
    if (collision.Other.lock()->GetEntity()->GetTag() == GameConstants::OBSTACLE_TAG) {
        _GameManager->OnPlayerLostLife();
        Phezu::Destroy(m_Entity);
    }
}
void Ball::OnCollisionExit(const Phezu::Collision& collision) {
    
}
