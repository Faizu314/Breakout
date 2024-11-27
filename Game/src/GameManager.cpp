#include "Game.hpp"
#include "GameManager.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "HealthBar.hpp"
#include "Brick.hpp"

const int TOTAL_LIVES = 3;

void GameManager::Start() {
    m_CurrentLevel = 0;
    
    Phezu::SubscribeToOnSceneLoaded(this, &GameManager::OnSceneLoaded);
    Phezu::LoadScene(GetLevelName(m_CurrentLevel));
    
    m_CurrentLives = TOTAL_LIVES;
}

void GameManager::OnSceneLoaded() {
    m_HasRoundStarted = false;
    
    auto playerEntity = Phezu::CreateEntity(m_PlayerPrefabID).lock();
    m_Player = playerEntity->GetTransformData();
    m_Player->SetLocalPosition(GameConstants::PLAYER_START_POSITION);
    
    auto healthBarEntity = Phezu::CreateEntity(m_HealthBarPrefabID).lock();
    healthBarEntity->GetTransformData()->SetLocalPosition(GameConstants::HEALTH_BAR_START_POSITION);
    m_HealthBar = healthBarEntity->GetComponent<HealthBar>().lock();
    m_HealthBar->OnLifeChanged(m_CurrentLives);
    
    auto ball = Phezu::CreateEntity(m_BallPrefabID).lock();
    ball->GetTransformData()->SetLocalPosition(GameConstants::BALL_START_POSITION);
    auto ballBehaviour = ball->GetComponent<Ball>().lock();
    ballBehaviour->_GameManager = this;
    m_Ball = ball->GetPhysicsData().lock();
    
    LoadBricks(GameConstants::BRICK_PADDING, GameConstants::BRICK_SPACING, GameConstants::BRICKS_OFFSET);
}

void GameManager::Update(float deltaTime) {
    if (m_HasRoundStarted)
        return;
    
    auto input = Phezu::GetInput();
    
    if (input.Space) {
        m_Ball->Velocity = Phezu::Random::Direction(70, 110) * GameConstants::BALL_MOVEMENT_SPEED;
        m_HasRoundStarted = true;
    }
}

void GameManager::LoadBricks(Phezu::Vector2 padding, Phezu::Vector2 spacing, Phezu::Vector2 offset) {
    Phezu::Vector2 brickSize
    (
       (GameConstants::SCREEN_SIZE.X() - (padding.X() * 2) - ((GameConstants::BRICK_GRID_SIZE_X - 1) * spacing.X())) / GameConstants::BRICK_GRID_SIZE_X,
        GameConstants::BRICK_SIZE_Y
    );
    
    const char* levelData = GameConstants::LEVEL_DATA[m_CurrentLevel];
    float startX = (-GameConstants::SCREEN_SIZE.X() / 2) + padding.X() + (brickSize.X() / 2);
    Phezu::Vector2 gridPos(startX, (GameConstants::SCREEN_SIZE.Y() / 2) - padding.Y() - (brickSize.Y() / 2));
    
    m_CurrentBricksCount = 0;
    int index = 0;
    for (int y = 0; levelData[index] != '\0'; y++) {
        for (int x = 0; x < GameConstants::BRICK_GRID_SIZE_X && levelData[index] != '\0'; x++, index++) {
            uint64_t brickPrefabID;
            
            switch (levelData[index]) {
                case 'W':
                    brickPrefabID = m_WeakBrickPrefabID;
                    break;
                case 'N':
                    brickPrefabID = m_NormalBrickPrefabID;
                    break;
                case 'S':
                    brickPrefabID = m_StrongBrickPrefabID;
                    break;
                default:
                    brickPrefabID = -1;
                    break;
            }
            
            if (brickPrefabID != -1) {
                auto entity = Phezu::CreateEntity(brickPrefabID).lock();
                entity->GetTransformData()->SetLocalPosition(gridPos);
                entity->GetShapeData()->SetSize(brickSize);
                entity->GetComponent<Brick>().lock()->_GameManager = this;
                m_CurrentBricksCount++;
            }
            
            gridPos.SetX(gridPos.X() + brickSize.X() + spacing.X());
        }
        
        gridPos.Set(startX, gridPos.Y() - brickSize.Y() - spacing.Y());
    }
}

void GameManager::OnBrickBroken() {
    m_CurrentBricksCount--;
    
    if (m_CurrentBricksCount <= 0) {
        m_CurrentLevel++;
        Phezu::LoadScene(GetLevelName(m_CurrentLevel));
    }
}

void GameManager::OnPlayerLostLife() {
    m_HasRoundStarted = false;
    
    m_CurrentLives--;
    
    m_HealthBar->OnLifeChanged(m_CurrentLives);
    
    if (m_CurrentLives <= 0) {
        OnPlayerDead();
        return;
    }
    
    auto ball = Phezu::CreateEntity(m_BallPrefabID).lock();
    ball->GetTransformData()->SetLocalPosition(GameConstants::BALL_START_POSITION);
    auto ballBehaviour = ball->GetComponent<Ball>().lock();
    ballBehaviour->_GameManager = this;
    m_Ball = ball->GetPhysicsData().lock();
}

void GameManager::OnPlayerDead() {
    m_CurrentLives = TOTAL_LIVES;
    m_CurrentLevel = 0;
    Phezu::LoadScene(GetLevelName(0));
}
