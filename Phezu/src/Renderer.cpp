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
    
    void GetSdlRect(SDL_Rect& rect, Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
        int minX, maxX, minY, maxY;
        
        minX = glm::min(a.X(), glm::min(b.X(), glm::min(c.X(), d.X())));
        maxX = glm::max(a.X(), glm::max(b.X(), glm::max(c.X(), d.X())));
        
        minY = glm::min(a.Y(), glm::min(b.Y(), glm::min(c.Y(), d.Y())));
        maxY = glm::max(a.Y(), glm::max(b.Y(), glm::max(c.Y(), d.Y())));
        
        rect.x = minX;
        rect.y = minY;
        rect.w = maxX - minX;
        rect.h = maxY - minY;
    }
    
    Renderer::Renderer(const Window& window)
    : m_WorldToSdl(glm::mat3(1, 0, 0, 0, -1, 0, window.GetWidth() / 2.0,  window.GetHeight() / 2.0, 1))
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
        SDL_SetRenderTarget(m_RendererPtr, nullptr);
    }
    
    Renderer::~Renderer() {
        SDL_DestroyRenderer(m_RendererPtr);
    }

    Vector2 Renderer::WorldToSdlPosition(const Vector2& worldPos) const {
        glm::vec3 worldPos3(worldPos.X(), worldPos.Y(), 1.0);
        glm::vec3 sdlPos3 = m_WorldToSdl * worldPos3;
        return Vector2(glm::round(sdlPos3.x), glm::round(sdlPos3.y));
    }
    
    void Renderer::ClearFrame(const Color& bg) {
        SDL_Color color;
        bg.ConvertToSDLColor(color);
        SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_RendererPtr);
    }
    
    void Renderer::RenderUpdate(const std::vector<std::weak_ptr<Entity>>& renderableEntities, size_t count, const Color& bg) {
        ClearFrame(bg);
        
        int index = 0;
        for (auto entity : renderableEntities) {
            if (index == count)
                break;
            DrawEntity(entity);
            index++;
        }
        
        RenderFrame();
    }
    
    void Renderer::DrawEntity(std::weak_ptr<Entity> entity) {
        auto entityL = entity.lock();
        
        SDL_Rect dest;
        
        TransformData* transformData = entityL->GetTransformData();
        ShapeData* shapeData = entityL->GetShapeData();
        RenderData* renderData = entityL->GetRenderData();
        
        if (shapeData == nullptr || renderData == nullptr)
            return;
        
        Vector2 upLeftLocal = shapeData->GetVertexPosition(ShapeData::VertexType::UpLeft);
        Vector2 upRightLocal = shapeData->GetVertexPosition(ShapeData::VertexType::UpRight);
        Vector2 downRightLocal = shapeData->GetVertexPosition(ShapeData::VertexType::DownRight);
        Vector2 downLeftLocal = shapeData->GetVertexPosition(ShapeData::VertexType::DownLeft);
        
        Vector2 upLeftWorld = transformData->LocalToWorldPoint(upLeftLocal);
        Vector2 upRightWorld = transformData->LocalToWorldPoint(upRightLocal);
        Vector2 downRightWorld = transformData->LocalToWorldPoint(downRightLocal);
        Vector2 downLeftWorld = transformData->LocalToWorldPoint(downLeftLocal);
        
        Vector2 upLeftSdl = WorldToSdlPosition(upLeftWorld);
        Vector2 upRightSdl = WorldToSdlPosition(upRightWorld);
        Vector2 downRightSdl = WorldToSdlPosition(downRightWorld);
        Vector2 downLeftSdl = WorldToSdlPosition(downLeftWorld);
        
        GetSdlRect(dest, upLeftSdl, upRightSdl, downRightSdl, downLeftSdl);
        
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
        SDL_RenderPresent(m_RendererPtr);
    }
}
