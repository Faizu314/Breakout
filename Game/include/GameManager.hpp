#pragma once

#include "Phezu.hpp"

class GameManager : public Phezu::BehaviourComponent {
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
    void Update(float deltaTime) override {}
    void OnDestroy() override {}
public:
    int GetCurrentLives() { return m_CurrentLives; }
    void OnPlayerLostLife();
    void OnPlayerDead();
private:
    void OnSceneLoaded();
private:
    uint64_t m_PlayerPrefabID;
    uint64_t m_BallPrefabID;
private:
    Phezu::TransformData* m_Player;
    int m_CurrentLives;
    
    friend class GameManagerPrefab;
};

class GameManagerPrefab : public Phezu::BehaviourComponentPrefab<GameManager> {
    using Phezu::BehaviourComponentPrefab<GameManager>::BehaviourComponentPrefab;
public:
    uint64_t PlayerPrefabID;
    uint64_t BallPrefabID;
public:
    void InitRuntimeComponent(std::weak_ptr<Phezu::Scene> scene, std::shared_ptr<GameManager> component) const override {
        component->m_PlayerPrefabID = PlayerPrefabID;
        component->m_BallPrefabID = BallPrefabID;
    }
};
