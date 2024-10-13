#pragma once

#include "SDL2/SDL.h"

namespace Phezu {
    
    typedef SDL_Color Color;
    typedef SDL_Texture Texture;
    
    namespace Colors {
        inline const Color WHITE = SDL_Color{ 255, 255, 255, 255};
    }
    
    //void SDL_Texture* LoadTexture(const Renderer& renderer);
    
    class Renderer {
        
    };
}
