#include <iostream>
#include "Game.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

int main(int argc, const char* argv[]) {
    engine.Init("Phezu", GameConstants::SCREEN_SIZE.X(), GameConstants::SCREEN_SIZE.Y());
    
    PrepareScenes(engine);
    
    engine.Run();
}
