#include "Player.hpp"
#include "Game.hpp"

Player::Player(Phezu::Entity* entity, uint8_t componentID) : Phezu::BehaviourComponent(entity, componentID) {}

void Player::Start() {
    m_Transform = m_Entity->GetTransformData();
    m_RenderData = m_Entity->GetRenderData();
    auto physicsData = m_Entity->GetPhysicsData().lock();
    physicsData->RegisterOnCollisionEnter(this, &Player::OnCollisionEnter);
    physicsData->RegisterOnCollisionExit(this, &Player::OnCollisionExit);
}

void Player::Update(float deltaTime) {
    const auto& input = Phezu::GetInput();

    int moveDir = 0;
    
    if (input.A)
        moveDir = -1;
    else if (input.D)
        moveDir = 1;
    
    float currVelX = m_Velocity.X();
    float targetVelX = m_MaxSpeed * moveDir;
    float velocityX = currVelX + (Phezu::Sign(targetVelX - currVelX) * m_Acceleration * deltaTime);
    
    if (moveDir == 0 && currVelX * velocityX < 0)
        velocityX = 0;
    else
        velocityX = Phezu::Clamp(velocityX, -m_MaxSpeed, m_MaxSpeed);
    
    m_Velocity.SetX(velocityX);
    m_Transform->SetLocalPosition(m_Transform->GetLocalPosition() + (m_Velocity * deltaTime));
}

void Player::OnCollisionEnter(const Phezu::Collision& collision) {
    auto otherPhysics = collision.Other.lock();
    auto otherPos = otherPhysics->GetEntity()->GetTransformData()->GetLocalPosition();
    auto myPos = m_Transform->GetLocalPosition() + (Phezu::Vector2::Down * 40);
    
    float speed = otherPhysics->Velocity.Length();
    auto reflectionComponent = otherPhysics->Velocity / speed;
    auto positionComponent = (otherPos - myPos).Normalized();
    auto movementComponent = m_Velocity.Normalized();
    otherPhysics->Velocity = ((reflectionComponent * GameConstants::REFLECTION_WEIGHT) + (movementComponent * GameConstants::MOVEMENT_WEIGHT) + (positionComponent * GameConstants::POSITION_WEIGHT)).Normalized() * speed;
    
    m_RenderData->Tint = Phezu::Color(255, 0, 0, 255);
    
    if (otherPhysics->GetEntity()->GetTag() != GameConstants::BALL_TAG)
        m_Velocity.SetX(0);
}

void Player::OnCollisionExit(const Phezu::Collision& collision) {
    m_RenderData->Tint = Phezu::Color(0, 255, 0, 255);
}
