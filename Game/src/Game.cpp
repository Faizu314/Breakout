#include "Game.hpp"

void PrepareScenes(Phezu::Engine& engine) {
    // Master Scene
    
    auto masterScene = engine.GetMasterScene().lock();
    
    auto gameManager = engine.CreatePrefab().lock();
    gameManager->RootEntity.IsRenderable = false;
    gameManager->RootEntity.IsCollidable = false;
    gameManager->RootEntity.AddComponentPrefab<GameManagerPrefab>();
    
    masterScene->CreateSceneEntity(gameManager->GetPrefabID());
    
    // Prefabs
    
    auto player = engine.CreatePrefab().lock();
    player->RootEntity.IsRenderable = true;
    player->RootEntity.IsCollidable = true;
    player->RootEntity.IsStatic = true;
    player->RootEntity.ShapeSizeOverride = Phezu::Vector2(80, 10);
    player->RootEntity.ShapePivotOverride = Phezu::Vector2(0, 0);
    player->RootEntity.TintOverride = Phezu::Color(0, 255, 0, 255);
    auto comp = player->RootEntity.AddComponentPrefab<PlayerPrefab>().lock();
    comp->MoveSpeed = 800;
    
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
    ball->RootEntity.IsRenderable = true;
    ball->RootEntity.IsCollidable = true;
    ball->RootEntity.IsStatic = false;
    ball->RootEntity.ShapeSizeOverride = Phezu::Vector2(10, 10);
    ball->RootEntity.TintOverride = Phezu::Color(0, 255, 0, 255);
    auto ballBehaviour = ball->RootEntity.AddComponentPrefab<BallBehaviourPrefab>().lock();
    ballBehaviour->StartVelocity = Phezu::Vector2(60, 300) * 3;
    
    auto brick = engine.CreatePrefab().lock();
    brick->RootEntity.IsRenderable = true;
    brick->RootEntity.IsCollidable = true;
    brick->RootEntity.IsStatic = true;
    brick->RootEntity.ShapeSizeOverride = Phezu::Vector2(60, 20);
    brick->RootEntity.AddComponentPrefab<BrickPrefab>();
    
    // Test scene
    
    auto scene = engine.CreateScene("Test Scene").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4));
    
    scene->CreateSceneEntity(brick->GetPrefabID(), Phezu::Vector2(0, 280));
    
    scene->CreateSceneEntity(player->GetPrefabID(), Phezu::Vector2(0, -280));
    scene->CreateSceneEntity(ball->GetPrefabID(), Phezu::Vector2(0, 0));
    
    // Test scene 2
    
    scene = engine.CreateScene("Test Scene 2").lock();
    
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(400 + 5, 0));
    scene->CreateSceneEntity(verticalWall->GetPrefabID(), Phezu::Vector2(-401 - 4, 0));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, 300 + 5));
    scene->CreateSceneEntity(horizontalWall->GetPrefabID(), Phezu::Vector2(0, -301 - 4));
    
    scene->CreateSceneEntity(brick->GetPrefabID(), Phezu::Vector2(-350, 280));
    
    scene->CreateSceneEntity(player->GetPrefabID(), Phezu::Vector2(0, -280));
    scene->CreateSceneEntity(ball->GetPrefabID(), Phezu::Vector2(0, 0));
}
