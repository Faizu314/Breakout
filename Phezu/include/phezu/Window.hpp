#pragma once

#include <string>

class SDL_Window;

namespace Phezu {
    
    class Window {
    public:
        Window(const std::string& name, int width, int height);
        ~Window();
    public:
        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
    public:
        operator SDL_Window*() const;
    private:
        SDL_Window* m_WindowPtr;
        const int m_Width;
        const int m_Height;
    };
}
