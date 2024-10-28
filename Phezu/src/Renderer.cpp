#include "Renderer.hpp"
#include "Window.hpp"
#include "scene/Entity.hpp"
#include "scene/components/TransformData.hpp"
#include "scene/components/ShapeData.hpp"
#include "scene/components/RenderData.hpp"
#include "maths/Math.hpp"

#include "SDL2/SDL.h"
#include "glm/glm.hpp"

namespace Phezu {

    const Color Color::White = Color(255, 255, 255, 255);
    
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
    
    Renderer::Renderer(const Window& window) 
    : m_WorldToSdl(glm::mat3(
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

    Vector2 Renderer::WorldToSdlPosition(const Vector2& worldPos) const {
        glm::vec3 worldPos3(worldPos.X(), worldPos.Y(), 1.0);
        glm::vec3 sdlPos3 = m_WorldToSdl * worldPos3;
        return Vector2(sdlPos3.x, sdlPos3.y);
    }
    
    void Renderer::ClearFrame(const Color& bg) {
        SDL_Color color;
        bg.ConvertToSDLColor(color);
        SDL_SetRenderDrawColor(m_RendererPtr, color.r, color.g, color.b, color.a);
        SDL_RenderClear(m_RendererPtr);
    }
    
    void Renderer::RenderUpdate(std::vector<std::weak_ptr<const Entity>>& entities, size_t count) {
        int index = 0;
        for (auto entity : entities) {
            if (index == count)
                break;
            DrawEntity(entity);
            index++;
        }
        
        RenderFrame();
    }
    
    void Renderer::DrawEntity(std::weak_ptr<const Entity> entity) {
        auto entityL = entity.lock();
        
        SDL_Rect dest;
        
        ShapeData* shapeData = entityL->GetShapeData();
        RenderData* renderData = entityL->GetRenderData();
        
        if (shapeData == nullptr || renderData == nullptr)
            return;
        
        Vector2 upLeftObj = shapeData->GetVertexObjectPosition(ShapeData::VertexType::UpLeft);
        Vector2 downRightObj = shapeData->GetVertexObjectPosition(ShapeData::VertexType::DownRight);
        Vector2 objWorldPos = entityL->GetTransformData().GetWorldPosition();
        
        Vector2 upLeftWorldPos = objWorldPos + upLeftObj;
        Vector2 downRightWorldPos = objWorldPos + downRightObj;
        
        Vector2 upLeftSdlPos = WorldToSdlPosition(upLeftWorldPos);
        Vector2 downRightSdlPos = WorldToSdlPosition(downRightWorldPos);
        
        dest.x = upLeftSdlPos.X();
        dest.y = upLeftSdlPos.Y();
        dest.w = downRightSdlPos.X() - upLeftSdlPos.X();
        dest.h = downRightSdlPos.Y() - upLeftSdlPos.Y();

        SDL_Texture* texture = nullptr; //TODO: set to pink error texture;
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
