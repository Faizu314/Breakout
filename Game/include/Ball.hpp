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
    Phezu::Vector2 m_StartVelocity;
    Phezu::RenderData* m_RenderData;
    
    friend class BallBehaviourPrefab;
};

class BallBehaviourPrefab : public Phezu::BehaviourComponentPrefab<Ball> {
public:
    using Phezu::BehaviourComponentPrefab<Ball>::BehaviourComponentPrefab;
public:
    Phezu::Vector2 StartVelocity;
public:
    void InitRuntimeComponent(std::weak_ptr<Phezu::Scene> scene, std::shared_ptr<Ball> component) const override {
        component->m_StartVelocity = StartVelocity;
    }
};
