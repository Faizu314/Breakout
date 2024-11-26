#pragma once

#include "Phezu.hpp"

class GameManager : public Phezu::BehaviourComponent {
    using Phezu::BehaviourComponent::BehaviourComponent;
public:
    void Start() override;
    void Update(float deltaTime) override;
    void OnDestroy() override {}
public:
    void OnPlayerLostLife();
    void OnPlayerDead();
    void OnBrickBroken();
private:
    void OnSceneLoaded();
    void LoadBricks(Phezu::Vector2 padding, Phezu::Vector2 spacing, Phezu::Vector2 offset);
private:
    uint64_t m_PlayerPrefabID;
    uint64_t m_BallPrefabID;
    uint64_t m_WeakBrickPrefabID;
    uint64_t m_NormalBrickPrefabID;
    uint64_t m_StrongBrickPrefabID;
private:
    Phezu::TransformData* m_Player;
    std::shared_ptr<Phezu::PhysicsData> m_Ball;
private:
    int m_CurrentLives;
    int m_CurrentLevel;
    int m_CurrentBricksCount;
    bool m_HasRoundStarted;
    
    friend class GameManagerPrefab;
};

class GameManagerPrefab : public Phezu::BehaviourComponentPrefab<GameManager> {
    using Phezu::BehaviourComponentPrefab<GameManager>::BehaviourComponentPrefab;
public:
    uint64_t PlayerPrefabID;
    uint64_t BallPrefabID;
    uint64_t WeakBrickPrefabID;
    uint64_t NormalBrickPrefabID;
    uint64_t StrongBrickPrefabID;
public:
    void InitRuntimeComponent(std::weak_ptr<Phezu::Scene> scene, std::shared_ptr<GameManager> component) const override {
        component->m_PlayerPrefabID = PlayerPrefabID;
        component->m_BallPrefabID = BallPrefabID;
        component->m_WeakBrickPrefabID = WeakBrickPrefabID;
        component->m_NormalBrickPrefabID = NormalBrickPrefabID;
        component->m_StrongBrickPrefabID = StrongBrickPrefabID;
    }
};
