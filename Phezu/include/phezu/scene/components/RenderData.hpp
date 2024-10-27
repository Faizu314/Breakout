#pragma once

#include "Renderer.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        Color Tint;
        std::weak_ptr<Texture> Sprite;
        RenderData(Entity* entity) : DataComponent(entity) {
            Tint = Color::White;
            //TODO: set a default white texture
        }
        RenderData(Entity* entity, Color tint) : DataComponent(entity) {
            Tint = tint;
        }
        RenderData(Entity* entity, std::weak_ptr<Texture> texture) : DataComponent(entity), Sprite(texture) {
            Tint = Color::White;
        }
        RenderData(Entity* entity, std::weak_ptr<Texture> texture, Color tint) : DataComponent(entity), Sprite(texture) {
            Tint = tint;
        }
    };
}
