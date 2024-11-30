#include "Game.hpp"

void PrepareScenes(Phezu::Engine& engine) {
    // Prefabs
    
    auto player = engine.CreatePrefab().lock();
    player->RootEntity.IsRenderable = true;
    player->RootEntity.IsCollidable = true;
    player->RootEntity.IsStatic = false;
    player->RootEntity.ShapeSizeOverride = Phezu::Vector2(80, 10);
    player->RootEntity.ShapePivotOverride = Phezu::Vector2(0, 0);
    player->RootEntity.TintOverride = Phezu::Color(0, 255, 0, 255);
    auto comp = player->RootEntity.AddComponentPrefab<PlayerPrefab>().lock();
    comp->MaxSpeed = GameConstants::PLAYER_MOVEMENT_SPEED;
    comp->Acceleration = GameConstants::PLAYER_ACCELERATION;
    
    auto healthBar = engine.CreatePrefab().lock();
    healthBar->RootEntity.AddComponentPrefab<HealthBarPrefab>();
    
    auto& heart1 = healthBar->RootEntity.CreateChildEntity();
    heart1.IsRenderable = true;
    heart1.PositionOverride = Phezu::Vector2(0, 0);
    heart1.ShapeSizeOverride = Phezu::Vector2(20, 20);
    auto& heart2 = healthBar->RootEntity.CreateChildEntity();
    heart2.IsRenderable = true;
    heart2.PositionOverride = Phezu::Vector2(30, 0);
    heart2.ShapeSizeOverride = Phezu::Vector2(20, 20);
    auto& heart3 = healthBar->RootEntity.CreateChildEntity();
    heart3.IsRenderable = true;
    heart3.PositionOverride = Phezu::Vector2(60, 0);
    heart3.ShapeSizeOverride = Phezu::Vector2(20, 20);
    
    auto verticalWall = engine.CreatePrefab().lock();
    verticalWall->RootEntity.IsRenderable = true;
    verticalWall->RootEntity.IsCollidable = true;
    verticalWall->RootEntity.IsStatic = true;
    verticalWall->RootEntity.ShapeSizeOverride = Phezu::Vector2(10, 600);
    verticalWall->RootEntity.TintOverride = Phezu::Color(200, 0, 200, 255);
    
    auto horizontalWall = engine.CreatePrefab().lock();
    horizontalWall->RootEntity.IsRenderable = true;
    horizontalWall->RootEntity.IsCollidable = true;
    horizontalWall->RootEntity.IsStatic = true;
    horizontalWall->RootEntity.ShapeSizeOverride = Phezu::Vector2(800, 10);
    horizontalWall->RootEntity.TintOverride = Phezu::Color(200, 0, 200, 255);
    
    auto ball = engine.CreatePrefab().lock();
    ball->RootEntity.TagOverride = GameConstants::BALL_TAG;
    ball->RootEntity.IsRenderable = true;
    ball->RootEntity.IsCollidable = true;
    ball->RootEntity.IsStatic = false;
    ball->RootEntity.ShapeSizeOverride = Phezu::Vector2(10, 10);
    ball->RootEntity.TintOverride = Phezu::Color(0, 255, 0, 255);
    ball->RootEntity.AddComponentPrefab<BallBehaviourPrefab>();
    
    auto weakBrick = engine.CreatePrefab().lock();
    weakBrick->RootEntity.IsRenderable = true;
    weakBrick->RootEntity.IsCollidable = true;
    weakBrick->RootEntity.IsStatic = true;
    weakBrick->RootEntity.ShapeSizeOverride = Phezu::Vector2(60, 20);
    auto brickBehaviour = weakBrick->RootEntity.AddComponentPrefab<BrickPrefab>().lock();
    brickBehaviour->TotalHealth = 1;
    
    auto normalBrick = engine.CreatePrefab().lock();
    normalBrick->RootEntity.IsRenderable = true;
    normalBrick->RootEntity.IsCollidable = true;
    normalBrick->RootEntity.IsStatic = true;
    normalBrick->RootEntity.ShapeSizeOverride = Phezu::Vector2(60, 20);
    brickBehaviour = normalBrick->RootEntity.AddComponentPrefab<BrickPrefab>().lock();
    brickBehaviour->TotalHealth = 2;
    
    auto strongBrick = engine.CreatePrefab().lock();
    strongBrick->RootEntity.IsRenderable = true;
    strongBrick->RootEntity.IsCollidable = true;
    strongBrick->RootEntity.IsStatic = true;
    strongBrick->RootEntity.ShapeSizeOverride = Phezu::Vector2(60, 20);
    brickBehaviour = strongBrick->RootEntity.AddComponentPrefab<BrickPrefab>().lock();
    brickBehaviour->TotalHealth = 3;
    
    // Master Scene
    
    auto masterScene = engine.GetMasterScene().lock();
    
    auto gameManager = engine.CreatePrefab().lock();
    gameManager->RootEntity.IsRenderable = false;
    gameManager->RootEntity.IsCollidable = false;
    auto gmBehaviourL = gameManager->RootEntity.AddComponentPrefab<GameManagerPrefab>().lock();
    gmBehaviourL->PlayerPrefabID = player->GetPrefabID();
    gmBehaviourL->BallPrefabID = ball->GetPrefabID();
    gmBehaviourL->WeakBrickPrefabID = weakBrick->GetPrefabID();
    gmBehaviourL->NormalBrickPrefabID = normalBrick->GetPrefabID();
    gmBehaviourL->StrongBrickPrefabID = strongBrick->GetPrefabID();
    gmBehaviourL->HealthBarPrefabID = healthBar->GetPrefabID();
    
    masterScene->CreateSceneEntity(gameManager->GetPrefabID());
    
    // Level 1
    
    auto scene = engine.CreateScene("Level 1").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
    
    // Level 2
    
    scene = engine.CreateScene("Level 2").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
    
    // Level 3
    
    scene = engine.CreateScene("Level 3").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
    
    // Level 4
    
    scene = engine.CreateScene("Level 4").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
    
    // Level 5
    
    scene = engine.CreateScene("Level 5").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
    
    // Level 6
    
    scene = engine.CreateScene("Level 6").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
    
    // Level 7
    
    scene = engine.CreateScene("Level 7").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4), GameConstants::OBSTACLE_TAG);
}
