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
    class Vector2;
    
    class Texture {
    private:
        Texture() = delete;
        Texture(SDL_Texture* texture);
    private:
        SDL_Texture* m_Texture;
    public:
        operator SDL_Texture*() { return m_Texture; }
        
        friend class Renderer;
    };
    
    class QuadUVs {
    public:
        QuadUVs(float x = 0, float y = 0, float w = 1, float h = 1);
    public:
        float x, y, w, h;
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
        Vector2 WorldToSdlPosition(const Vector2& worldPos) const;
    private:
        SDL_Renderer* m_RendererPtr;
        glm::mat3 m_WorldToSdl;
        
        friend class Engine;
    };
    
    std::shared_ptr<Texture> LoadTexture(const Renderer& renderer);
}
