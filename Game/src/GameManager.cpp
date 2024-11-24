#include "GameManager.hpp"
#include "Player.hpp"
#include "Ball.hpp"

const int TOTAL_LIVES = 3;

void GameManager::Start() {
    Phezu::SubscribeToOnSceneLoaded(this, &GameManager::OnSceneLoaded);
    Phezu::LoadScene("Level 1");
    
    m_CurrentLives = TOTAL_LIVES;
}

void GameManager::OnSceneLoaded() {
    auto player = Phezu::CreateEntity(m_PlayerPrefabID).lock();
    m_Player = player->GetTransformData();
    m_Player->SetLocalPosition(Phezu::Vector2(0, -280));
    
    auto ball = Phezu::CreateEntity(m_BallPrefabID).lock();
    ball->GetTransformData()->SetLocalPosition(Phezu::Vector2(0, 0));
    auto ballBehaviour = ball->GetComponent<Ball>().lock();
    ballBehaviour->_GameManager = this;
}

void GameManager::OnPlayerLostLife() {
    m_CurrentLives--;
    
    if (m_CurrentLives <= 0) {
        OnPlayerDead();
        return;
    }
    
    auto ball = Phezu::CreateEntity(m_BallPrefabID).lock();
    ball->GetTransformData()->SetLocalPosition(Phezu::Vector2(0, 0));
    auto ballBehaviour = ball->GetComponent<Ball>().lock();
    ballBehaviour->_GameManager = this;
    
    m_Player->SetLocalPosition(Phezu::Vector2(0, -280));
}

void GameManager::OnPlayerDead() {
    Phezu::LoadScene("Level 1");
    
    m_CurrentLives = TOTAL_LIVES;
}
