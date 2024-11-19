#include "Renderer.hpp"
#include "Window.hpp"
#include "scene/Entity.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "maths/Math.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "glm/glm.hpp"

namespace Phezu {

    const Color Color::White = Color(255, 255, 255, 255);
    const Color Color::Black = Color(0, 0, 0, 255);
    
    Texture::Texture(SDL_Texture* texture) : m_Texture(texture) {}
    QuadUVs::QuadUVs(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}
    
    void Color::ConvertToSDLColor(SDL_Color &sdlColor) const {
        sdlColor.r = r;
        sdlColor.g = g;
        sdlColor.b = b;
        sdlColor.a = a;
    }
    
    Color Color::FromSDLColor(const SDL_Color &sdlColor) {
        return Color(sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
    }
    
    void GetSdlRect(SDL_Rect& rect, Vector2 a, Vector2 b) {
        int minX, maxX, minY, maxY;
        
        minX = glm::min(a.X(), b.X());
        maxX = glm::max(a.X(), b.X());
        
        minY = glm::min(a.Y(), b.Y());
        maxY = glm::max(a.Y(), b.Y());
        
        rect.x = minX;
        rect.y = minY;
        rect.w = maxX - minX;
        rect.h = maxY - minY;
    }
    
    Renderer::Renderer(Engine* engine, const Window& window)
    : m_Engine(engine), m_WorldToSdl(glm::mat3(1, 0, 0, 0, -1,  0,  window.GetWidth() / 2.0,  window.GetHeight() / 2.0, 1))
    {
        int renderersFlag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

        m_RendererPtr = SDL_CreateRenderer(window, -1, renderersFlag);

        if (!m_RendererPtr)
        {
            //TODO: Logging::Log("Failed to create renderer: %s\n", SDL_GetError());
            exit(1);
        }
        
        m_DefaultTex = SDL_CreateTexture(m_RendererPtr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
        SDL_SetRenderTarget(m_RendererPtr, m_DefaultTex);
        SDL_SetRenderDrawColor(m_RendererPtr, 255, 255, 255, 255);
        SDL_RenderClear(m_RendererPtr);
        
        m_IntermediateTex = SDL_CreateTexture(m_RendererPtr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, window.GetWidth(), window.GetHeight());
        SDL_SetRenderTarget(m_RendererPtr, m_IntermediateTex);
    }
    
    Renderer::~Renderer() {
        SDL_DestroyRenderer(m_RendererPtr);
    }
    
    Vector2 Renderer::WorldToSdlPosition(const Vector2& worldPos) const {
        int roundedX = RoundToPixel(worldPos.X());
        int roundedY = RoundToPixel(worldPos.Y());
        
        glm::vec3 worldPos3(roundedX, roundedY, 1.0);
        glm::vec3 sdlPos3 = m_WorldToSdl * worldPos3;
        
        return Vector2(sdlPos3.x, sdlPos3.y);
    }
    
    void Renderer::ClearFrame(const Color& bg) {
        SDL_Color color;
        bg.ConvertToSDLColor(color);
        SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_RendererPtr);
    }
    
    void Renderer::DrawEntities(const std::vector<std::weak_ptr<Entity>>& renderableEntities, size_t count, const Color& bg) {
        int index = 0;
        for (auto entity : renderableEntities) {
            if (index >= count)
                break;
            DrawEntity(entity);
            index++;
        }
    }
    
    void Renderer::DrawEntity(std::weak_ptr<Entity> entity) {
        auto entityL = entity.lock();
        
        SDL_Rect dest;
        
        TransformData* transformData = entityL->GetTransformData();
        ShapeData* shapeData = entityL->GetShapeData();
        RenderData* renderData = entityL->GetRenderData();
        
        if (shapeData == nullptr || renderData == nullptr)
            return;
        
        Vector2 upRightLocal = shapeData->GetVertexPosition(ShapeData::VertexType::UpRight);
        Vector2 downLeftLocal = shapeData->GetVertexPosition(ShapeData::VertexType::DownLeft);
        
        Vector2 upRightWorld = transformData->LocalToWorldPoint(upRightLocal);
        Vector2 downLeftWorld = transformData->LocalToWorldPoint(downLeftLocal);
        
        Vector2 upRightSdl = WorldToSdlPosition(upRightWorld);
        Vector2 downLeftSdl = WorldToSdlPosition(downLeftWorld);
        
        GetSdlRect(dest, downLeftSdl, upRightSdl);
        
        SDL_Texture* texture = m_DefaultTex;
        SDL_Color tint;
        entityL->GetRenderData()->Tint.ConvertToSDLColor(tint);
        
        if (auto tex = entityL->GetRenderData()->Sprite.lock())
            texture = *tex.get();
        
        SDL_Rect srcRect;
        QuadUVs uvs = entityL->GetRenderData()->RectUVs;
        srcRect.x = uvs.x;
        srcRect.y = uvs.y;
        srcRect.w = uvs.w;
        srcRect.h = uvs.h;
        
        SDL_SetTextureColorMod(texture, tint.r, tint.g, tint.b);
        SDL_RenderCopy(m_RendererPtr, texture, &srcRect, &dest);
    }
    
    void Renderer::RenderFrame() {
        SDL_SetRenderTarget(m_RendererPtr, nullptr);
        SDL_RenderCopy(m_RendererPtr, m_IntermediateTex, nullptr, nullptr);
        SDL_RenderPresent(m_RendererPtr);
        SDL_SetRenderTarget(m_RendererPtr, m_IntermediateTex);
    }
}
