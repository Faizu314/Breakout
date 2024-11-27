#pragma once

#include "Phezu.hpp"
#include "Player.hpp"
#include "HealthBar.hpp"
#include "Ball.hpp"
#include "Brick.hpp"
#include "GameManager.hpp"

void PrepareScenes(Phezu::Engine& engine);

inline std::string GetLevelName(int levelNo) { return std::string("Level ") + std::to_string(levelNo + 1); }

struct GameConstants {
    inline static const Phezu::Vector2 SCREEN_SIZE = Phezu::Vector2(800, 600);
    
    inline static const char* OBSTACLE_TAG = "Obstacle";
    inline static const char* BALL_TAG = "Ball";
    
    inline static const Phezu::Vector2 PLAYER_START_POSITION = Phezu::Vector2(0, -240);
    inline static constexpr float PLAYER_MOVEMENT_SPEED = 1000;
    inline static constexpr float PLAYER_ACCELERATION = 12000;
    
    inline static const Phezu::Vector2 HEALTH_BAR_START_POSITION = Phezu::Vector2(-370, -270);
    
    inline static const Phezu::Vector2 BALL_START_POSITION = Phezu::Vector2(0, -100);
    inline static constexpr float BALL_MOVEMENT_SPEED = 500;
    inline static constexpr float REFLECTION_WEIGHT = 0;
    inline static constexpr float MOVEMENT_WEIGHT = 0;
    inline static constexpr float POSITION_WEIGHT = 1;
    
    inline static constexpr int BRICK_GRID_SIZE_X = 11;
    inline static constexpr float BRICK_SIZE_Y = 15;
    inline static const Phezu::Vector2 BRICK_SPACING = Phezu::Vector2(5, 5);
    inline static const Phezu::Vector2 BRICK_PADDING = Phezu::Vector2(5, 0);
    inline static const Phezu::Vector2 BRICKS_OFFSET = Phezu::Vector2(0, 0);
    
    // O => Empty, S => Strong Brick, N => Normal Brick, W => Weak Brick
    inline static const char* LEVEL_DATA[10] = {
        
        
        "-----------"
        "-----------"
        "-----------"
        "-----------"
        "-----------"
        "-WWWWWWWWW-"
        "-WWWWWWWWW-",
        
        
        "-----------"
        "-----------"
        "-----------"
        "-----------"
        "-----------"
        "-WWWWWWWWW-"
        "-W-------W-"
        "-W-------W-"
        "-W-------W-"
        "-WNNNNNNNW-",

        
        "-----------"
        "-----------"
        "-----------"
        "-W-W-W-W-W-"
        "-----------"
        "W-W-W-W-W-W"
        "-----------"
        "-W-W-W-W-W-"
        "-----------"
        "W-W-W-W-W-W"
        "-----------"
        "-W-W-W-W-W-"
        "-----------"
        "W-W-W-W-W-W",

        "-----------"
        "-W-W-W-W-W-"
        "-----------"
        "W-W-W-W-W-W"
        "-----------"
        "-W-W-W-W-W-"
        "-----------"
        "W-W-W-W-W-W"
        "-----------"
        "-W-W-W-W-W-"
        "-----------"
        "-----------"
        "NNNNNNNNNNN",
        
        "-----------"
        "-----------"
        "-----------"
        "-----------"
        "--W-----W--"
        "-W-W---W-W-"
        "-W-W---W-W-"
        "--W--W--W--"
        "----W-W----"
        "--W--W--W--"
        "-W-W---W-W-"
        "-W-W---W-W-"
        "--W-----W--"
        "-----------"
        "NNNNNNNNNNN",

        "-----------"
        "-----------"
        "-----------"
        "WWWWWWWWWWW"
        "-----------"
        "-----------"
        "-----------"
        "WWWWWWWWWWW"
        "-----------"
        "-----------"
        "-----------"
        "WWWWWWWWWWW"
        "-----------"
        "-----------"
        "-----------"
        "WWWWWWWWWWW",

        "-----------"
        "W---------W"
        "-W-------W-"
        "W-W-----W-W"
        "-W-W-W-W-W-"
        "W-W-W-W-W-W"
        "-W-W-W-W-W-"
        "--W-W-W-W--"
        "---W-W-W---"
        "--W-W-W-W--"
        "-W-W-W-W-W-"
        "W-W-W-W-W-W"
        "-W-W---W-W-"
        "W-W-----W-W"
        "-W-------W-"
        "W---------W",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",

        "WWWWWWWWW"
        "WWWWWWWWW",
    };
};
