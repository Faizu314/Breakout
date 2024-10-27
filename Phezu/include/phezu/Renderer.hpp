#pragma once

#include <cstdint>

namespace Phezu {
    
    class SDL_Texture;
    
    class Color {
    public:
        Color(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}
        uint8_t r, g, b, a;
    public:
        static const Color White;
    private:
        void ConvertToSDLColor(struct SDL_Color& sdlColor) const;
        static Color FromSDLColor(const struct SDL_Color& sdlColor);
        
        friend class Renderer;
    };
    
    typedef SDL_Texture Texture;
    
    //void SDL_Texture* LoadTexture(const Renderer& renderer);
    
    class Renderer {
        
    };
}
