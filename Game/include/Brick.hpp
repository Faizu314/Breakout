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
    Phezu::RenderData* m_RenderData;
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
    void InitRuntimeComponent(std::weak_ptr<Phezu::Scene> scene, uint64_t instanceID) const override {
        auto eL = GetRuntimeComponent(scene, instanceID).lock();
        eL->m_TotalHealth = TotalHealth;
    }
};
