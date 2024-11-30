#include "Game.hpp"

Phezu::Engine& engine = Phezu::CreateEngine();

#ifdef __APPLE__

int main(int argc, const char* argv[]) {
    engine.Init("Phezu", GameConstants::SCREEN_SIZE.X(), GameConstants::SCREEN_SIZE.Y());
    
    PrepareScenes(engine);
    
    engine.Run();
}

#elif _WIN32

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    engine.Init("Phezu", GameConstants::SCREEN_SIZE.X(), GameConstants::SCREEN_SIZE.Y());

    PrepareScenes(engine);

    engine.Run();

    return 0;
}

#endif
