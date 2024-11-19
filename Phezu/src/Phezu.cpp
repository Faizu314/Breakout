#include "Phezu.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

namespace Phezu {
    
    static Engine* s_Instance = nullptr;
    
    Engine& CreateEngine() {
        if (s_Instance == nullptr)
            s_Instance = new Engine();
        
        return *s_Instance;
    }
    
    void LoadScene(const std::string& sceneName) {
        if (s_Instance == nullptr) {
            //TODO: Logging
            return;
        }
        
        s_Instance->LoadScene(sceneName);
    }
    
    long long unsigned int GetFrameCount() {
        if (s_Instance == nullptr) {
            //TODO: Logging
            return 0;
        }
        
        return s_Instance->GetFrameCount();
    }
}
