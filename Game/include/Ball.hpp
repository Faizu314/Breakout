#pragma once

#include "Phezu.hpp"
#include "GameManager.hpp"

class Ball : public Phezu::BehaviourComponent {
public:
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
    void Update(float deltaTime) override {}
    void OnDestroy() override {}
    void OnCollisionEnter(const Phezu::Collision& collision);
    void OnCollisionExit(const Phezu::Collision& collision);
public:
    GameManager* _GameManager;
private:
    Phezu::RenderData* m_RenderData;
    
    friend class BallBehaviourPrefab;
};

class BallBehaviourPrefab : public Phezu::BehaviourComponentPrefab<Ball> {
    using Phezu::BehaviourComponentPrefab<Ball>::BehaviourComponentPrefab;
};
