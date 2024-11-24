#include <iostream>
#include "Game.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

int main(int argc, const char* argv[]) {
    engine.Init("Phezu", 800, 600);
    
    PrepareScenes(engine);
    
    engine.Run();
}
