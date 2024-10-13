#pragma once

#include "Renderer.hpp"
#include "scene/components/DataComponent.hpp"

namespace Phezu {
    
    class RenderData : public DataComponent {
    public:
        Color Tint;
        Texture* GetTexture();
        RenderData(std::shared_ptr<Entity> entity) : DataComponent(entity) {
            Tint = Colors::WHITE;
        }
        RenderData(std::shared_ptr<Entity> entity, Color tint) : DataComponent(entity) {
            Tint = tint;
        }
        RenderData(std::shared_ptr<Entity> entity, Texture* texture) : DataComponent(entity), m_Texture(texture) {
            Tint = Colors::WHITE;
        }
        RenderData(std::shared_ptr<Entity> entity, Texture* texture, Color tint) : DataComponent(entity), m_Texture(texture) {
            Tint = tint;
        }
    private:
        const Texture* m_Texture;
    };
}
