#pragma once

#include "Phezu.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "Brick.hpp"
#include "GameManager.hpp"

void PrepareScenes(Phezu::Engine& engine);

struct GameConstants {
    inline static const char* OBSTACLE_TAG = "Obstacle";
    
    inline static constexpr float PLAYER_MOVEMENT_SPEED = 800;
    inline static constexpr float BALL_MOVEMENT_SPEED = 612;
};
