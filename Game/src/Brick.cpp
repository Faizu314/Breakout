#include "Brick.hpp"

void Brick::Start() {
    m_PhysicsData = m_Entity->GetPhysicsData().lock();
    m_PhysicsData->RegisterOnCollisionEnter(this, &Brick::OnCollisionEnter);
}
void Brick::OnDestroy() {
    m_PhysicsData->UnregisterOnCollisionEnter(this);
    Phezu::LoadScene("Test Scene 2");
}
void Brick::OnCollisionEnter(const Phezu::Collision& collision) {
    Phezu::Destroy(m_Entity);
}
