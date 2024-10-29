#pragma once

#include "Renderer.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        RenderData(Entity* entity) : DataComponent(entity), Tint(Color::White) {}
        RenderData(Entity* entity, Color tint) : DataComponent(entity), Tint(tint) {}
    public:
        RenderData(Entity* entity, std::weak_ptr<Texture> texture) : DataComponent(entity), Sprite(texture), Tint(Color::White) {}
        RenderData(Entity* entity, std::weak_ptr<Texture> texture, Color tint) : DataComponent(entity), Sprite(texture), Tint(tint) {}
        RenderData(Entity* entity, std::weak_ptr<Texture> texture, QuadUVs uvs) : DataComponent(entity), Sprite(texture), RectUVs(uvs), Tint(Color::White) {}
        RenderData(Entity* entity, std::weak_ptr<Texture> texture, Color tint, QuadUVs uvs) : DataComponent(entity), Sprite(texture), Tint(tint), RectUVs(uvs) {}
    public:
        std::weak_ptr<Texture> Sprite;
        QuadUVs RectUVs;
        Color Tint;
    };
}
