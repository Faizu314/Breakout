#include "Ball.hpp"

void Ball::Start() {
    m_RenderData = m_Entity->GetRenderData();
    
    auto physicsData = m_Entity->GetPhysicsData().lock();
    physicsData->Velocity = m_StartVelocity;
    
    physicsData->RegisterOnCollisionEnter(this, &Ball::OnCollisionEnter);
    physicsData->RegisterOnCollisionExit(this, &Ball::OnCollisionExit);
}

void Ball::OnCollisionEnter(const Phezu::Collision& collision) {
    m_RenderData->Tint = Phezu::Color(255, 0, 0, 255);
}
void Ball::OnCollisionExit(const Phezu::Collision& collision) {
    m_RenderData->Tint = Phezu::Color(0, 255, 0, 255);
}
