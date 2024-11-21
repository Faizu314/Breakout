#pragma once

#include "Phezu.hpp"

class Brick : public Phezu::BehaviourComponent {
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
    void Update(float deltaTime) override {}
    void OnDestroy() override;
    void OnCollisionEnter(const Phezu::Collision& collision);
private:
    std::shared_ptr<Phezu::PhysicsData> m_PhysicsData;
};

class BrickPrefab : public Phezu::BehaviourComponentPrefab<Brick> {
    using Phezu::BehaviourComponentPrefab<Brick>::BehaviourComponentPrefab;
};
