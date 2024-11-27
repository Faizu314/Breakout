#pragma once

#include "Phezu.hpp"

class GameManager;

class Brick : public Phezu::BehaviourComponent {
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
    void Update(float deltaTime) override {}
    void OnDestroy() override;
    void OnCollisionEnter(const Phezu::Collision& collision);
private:
    std::shared_ptr<Phezu::PhysicsData> m_PhysicsData;
    Phezu::RenderData* m_RenderData;
public:
    GameManager* _GameManager;
private:
    int m_TotalHealth;
    int m_CurrentHealth;
    
    friend class BrickPrefab;
};

class BrickPrefab : public Phezu::BehaviourComponentPrefab<Brick> {
    using Phezu::BehaviourComponentPrefab<Brick>::BehaviourComponentPrefab;
public:
    int TotalHealth;
public:
    void InitRuntimeComponent(std::weak_ptr<Phezu::Scene> scene, std::shared_ptr<Brick> component) const override {
        component->m_TotalHealth = TotalHealth;
    }
};
