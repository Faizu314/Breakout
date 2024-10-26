#include "Game.hpp"
#include "Phezu.hpp"
#include "scene/Prefab.hpp"
#include "scene/PrefabEntity.hpp"
#include "scene/Scene.hpp"

#if __APPLE__

int main(int argc, const char* argv[]) {
    Phezu::Engine engine;
    
    auto prefab = engine.CreatePrefab().lock();
    
    auto scene = engine.CreateScene("Test Scene").lock();
    scene->CreateSceneEntity(prefab->RootEntity.GetPrefabEntityID());
    
    engine.Init();
    engine.Run();
}

#endif
