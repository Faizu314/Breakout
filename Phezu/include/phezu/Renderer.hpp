#pragma once

#include <cstdint>
#include <memory>

#include "glm/glm.hpp"

class SDL_Color;
class SDL_Texture;
class SDL_Window;
class SDL_Renderer;

namespace Phezu {
    
    class Window;
    class Entity;
    
    class Texture {
    private:
        Texture() = delete;
        Texture(SDL_Texture* texture);
    private:
        SDL_Texture* m_Texture;
        
        friend class Renderer;
    };
    
    class QuadUVs {
    public:
        QuadUVs(float x0 = 0, float x1 = 1, float y0 = 0, float y1 = 1);
    public:
        float x0, x1, y0, y1;
    };
    
    class Color {
    public:
        Color(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255);
        uint8_t r, g, b, a;
    public:
        static const Color White;
    private:
        void ConvertToSDLColor(SDL_Color& sdlColor) const;
        static Color FromSDLColor(const SDL_Color& sdlColor);
        
        friend class Renderer;
    };
    
    class Renderer {
    public:
        Renderer() = delete;
        Renderer(const Window& window);
        ~Renderer();
    public:
        void ClearFrame(const Color& bg);
        void DrawEntity(std::weak_ptr<const Entity> entity);
        void RenderFrame();
    private:
        void Blit();
    private:
        SDL_Renderer* m_RendererPtr;
        glm::mat3 m_PhezuToSdlScreenSpace;
        
        friend class Engine;
    };
    
    std::shared_ptr<Texture> LoadTexture(const Renderer& renderer);
}
