#pragma once

#include "Phezu.hpp"

class HealthBar : public Phezu::BehaviourComponent {
public:
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
public:
    void OnLifeChanged(int currLife);
private:
    std::shared_ptr<Phezu::Entity> m_Heart1;
    std::shared_ptr<Phezu::Entity> m_Heart2;
    std::shared_ptr<Phezu::Entity> m_Heart3;
};

class HealthBarPrefab : public Phezu::BehaviourComponentPrefab<HealthBar> {
    using Phezu::BehaviourComponentPrefab<HealthBar>::BehaviourComponentPrefab;
};
