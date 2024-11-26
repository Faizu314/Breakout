#pragma once

#include "Phezu.hpp"

//[GeneratePrefab]
class Player : public Phezu::BehaviourComponent {
public:
    Player(Phezu::Entity* entity, uint8_t componentID);
public:
    void Start() override;
    void Update(float deltaTime) override;
    void OnDestroy() override {}
public:
    void OnCollisionEnter(const Phezu::Collision& collision);
    void OnCollisionExit(const Phezu::Collision& collision);
private:
    //[PrefabOverride(MoveSpeed)]
    float m_MaxSpeed;
    float m_Acceleration;
private:
    Phezu::TransformData* m_Transform;
    Phezu::RenderData* m_RenderData;
    Phezu::Vector2 m_Velocity;
    
    friend class PlayerPrefab;
};

class PlayerPrefab : public Phezu::BehaviourComponentPrefab<Player> {
public:
    using Phezu::BehaviourComponentPrefab<Player>::BehaviourComponentPrefab;
public:
    float MaxSpeed;
    float Acceleration;
public:
    void InitRuntimeComponent(std::weak_ptr<Phezu::Scene> scene, std::shared_ptr<Player> component) const override {
        component->m_MaxSpeed = MaxSpeed;
        component->m_Acceleration = Acceleration;
    }
};
