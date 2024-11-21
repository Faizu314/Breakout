#pragma once

#include "Phezu.hpp"

class GameManager : public Phezu::BehaviourComponent {
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
    void Update(float deltaTime) override {}
    void OnDestroy() override {}
};

class GameManagerPrefab : public Phezu::BehaviourComponentPrefab<GameManager> {
    using Phezu::BehaviourComponentPrefab<GameManager>::BehaviourComponentPrefab;
};
