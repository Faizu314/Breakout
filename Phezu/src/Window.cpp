#include "Window.hpp"

#include "SDL2/SDL.h"

namespace Phezu {

    Window::Window(const std::string& name, int width, int height) : m_Width(width), m_Height(height) {
        int windowsFlag = 0;

        m_WindowPtr =
            SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowsFlag);

        if (!m_WindowPtr) {
            //TODO: Logging::Log("Failed to open %d x %d window: %s\n", width, height, SDL_GetError());
            exit(1);
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    }
    
    Window::~Window() {
        SDL_DestroyWindow(m_WindowPtr);
    }
    
    Window::operator SDL_Window*() const {
        return m_WindowPtr;
    }
}
