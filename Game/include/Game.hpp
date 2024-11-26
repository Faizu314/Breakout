#pragma once

#include "Phezu.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "Brick.hpp"
#include "GameManager.hpp"

void PrepareScenes(Phezu::Engine& engine);

inline std::string GetLevelName(int levelNo) { return std::string("Level ") + std::to_string(levelNo + 1); }

struct GameConstants {
    inline static const Phezu::Vector2 SCREEN_SIZE = Phezu::Vector2(800, 600);
    
    inline static const char* OBSTACLE_TAG = "Obstacle";
    
    inline static constexpr float PLAYER_MOVEMENT_SPEED = 800;
    inline static constexpr float BALL_MOVEMENT_SPEED = 600;
    
    inline static constexpr int BRICK_GRID_SIZE_X = 10;
    inline static constexpr float BRICK_SIZE_Y = 15;
    inline static const Phezu::Vector2 BRICK_SPACING = Phezu::Vector2(5, 5);
    inline static const Phezu::Vector2 BRICK_PADDING = Phezu::Vector2(5, 100);
    inline static const Phezu::Vector2 BRICKS_OFFSET = Phezu::Vector2(0, 0);
    
    // O => Empty, S => Strong Brick, N => Normal Brick, W => Weak Brick
    inline static const char* LEVEL_DATA[10] = {
        "WWWWWWWWWW"
        "-WWWWWWWW-"
        "-WWWWWWWW-",
        
        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW"
    };
};
