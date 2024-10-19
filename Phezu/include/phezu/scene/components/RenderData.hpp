#pragma once

#include "Renderer.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        Color Tint;
        std::weak_ptr<Texture> GetSprite();
        void SetSprite(std::weak_ptr<Texture> texture);
        RenderData(Entity* entity) : DataComponent(entity) {
            Tint = Colors::WHITE;
        }
        RenderData(Entity* entity, Color tint) : DataComponent(entity) {
            Tint = tint;
        }
        RenderData(Entity* entity, std::weak_ptr<Texture> texture) : DataComponent(entity), m_Sprite(texture) {
            Tint = Colors::WHITE;
        }
        RenderData(Entity* entity, std::weak_ptr<Texture> texture, Color tint) : DataComponent(entity), m_Sprite(texture) {
            Tint = tint;
        }
    private:
        std::weak_ptr<Texture> m_Sprite;
    };
}
