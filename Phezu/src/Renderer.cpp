#include "Renderer.hpp"
#include "Window.hpp"
#include "scene/Entity.hpp"

#include "SDL2/SDL.h"
#include "glm/glm.hpp"

namespace Phezu {
    
    Texture::Texture(SDL_Texture* texture) : m_Texture(texture) {}
    QuadUVs::QuadUVs(float x0, float x1, float y0, float y1) : x0(x0), x1(x1), y0(y0), y1(y1) {}
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}
    
    const Color Color::White = Color(255, 255, 255, 255);
    
    void Color::ConvertToSDLColor(SDL_Color &sdlColor) const {
        sdlColor.r = r;
        sdlColor.g = g;
        sdlColor.b = b;
        sdlColor.a = a;
    }
    
    Color Color::FromSDLColor(const SDL_Color &sdlColor) {
        return Color(sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    }
    
    Renderer::Renderer(const Window& window) 
    : m_PhezuToSdlScreenSpace(glm::mat3(
            1,  0, window.GetWidth() / 2.0,
            0, -1, window.GetHeight() / 2.0,
            0,  0, 1
        ))
    {
        int renderersFlag = SDL_RENDERER_ACCELERATED;

        m_RendererPtr = SDL_CreateRenderer(window, -1, renderersFlag);

        if (!m_RendererPtr)
        {
            //TODO: Logging::Log("Failed to create renderer: %s\n", SDL_GetError());
            exit(1);
        }
    }
    
    Renderer::~Renderer() {
        SDL_DestroyRenderer(m_RendererPtr);
    }
    
    void Renderer::ClearFrame(const Color& bg) {
        SDL_Color color;
        bg.ConvertToSDLColor(color);
        SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_RendererPtr);
    }
    
    void Renderer::DrawEntity(std::weak_ptr<const Entity> entity) {
        auto entityL = entity.lock();
        SDL_Rect dest;
        
        //dest.x = entityL->Render

        //SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
        //SDL_RenderCopy(_renderer, texture, &srcRect, &dest);
    }
    
    void Renderer::RenderFrame() {
        SDL_RenderPresent(m_RendererPtr);
    }
}
