#pragma once

#include "Renderer.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        Color Tint;
        std::weak_ptr<Texture> GetTexture();
        RenderData(const std::weak_ptr<Entity>& entity) : DataComponent(entity) {
            Tint = Colors::WHITE;
        }
        RenderData(const std::weak_ptr<Entity>& entity, Color tint) : DataComponent(entity) {
            Tint = tint;
        }
        RenderData(const std::weak_ptr<Entity>& entity, const std::weak_ptr<Texture>& texture) : DataComponent(entity), m_Texture(texture) {
            Tint = Colors::WHITE;
        }
        RenderData(const std::weak_ptr<Entity>& entity, const std::weak_ptr<Texture>& texture, Color tint) : DataComponent(entity), m_Texture(texture) {
            Tint = tint;
        }
    private:
        std::weak_ptr<Texture> m_Texture;
    };
}
