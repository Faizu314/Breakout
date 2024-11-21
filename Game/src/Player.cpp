#include "Player.hpp"

Player::Player(Phezu::Entity* entity, uint8_t componentID) : Phezu::BehaviourComponent(entity, componentID) {}

void Player::Start() {
    m_Transform = m_Entity->GetTransformData();
    m_RenderData = m_Entity->GetRenderData();
    auto physicsData = m_Entity->GetPhysicsData().lock();
    physicsData->RegisterOnCollisionEnter(this, &Player::OnCollisionEnter);
    physicsData->RegisterOnCollisionExit(this, &Player::OnCollisionExit);
}

void Player::Update(float deltaTime) {
    const Phezu::InputData& input = Phezu::Input::GetInput();

    m_MoveDir.Set(0, 0);
    
    if (input.A) {
        m_MoveDir.SetX(-1);
    }
    else if (input.D) {
        m_MoveDir.SetX(1);
    }
    
    m_MoveDir.Normalize();
    m_Transform->SetLocalPosition(m_Transform->GetLocalPosition() + (m_MoveDir * deltaTime * m_MoveSpeed));
}

void Player::OnCollisionEnter(const Phezu::Collision& collision) {
    m_RenderData->Tint = Phezu::Color(255, 0, 0, 255);
}

void Player::OnCollisionExit(const Phezu::Collision& collision) {
    m_RenderData->Tint = Phezu::Color(0, 255, 0, 255);
}
