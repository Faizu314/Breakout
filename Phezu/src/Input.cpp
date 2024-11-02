#include "Phezu.hpp"
#include "SDL2/SDL.h"

namespace Phezu {
    
    InputData Input::s_InputData;
    
    Input::Input(Engine* engine) : m_Engine(engine) {
        memset(&s_InputData, 0, sizeof(s_InputData));
    }
    
    void ProcessKeyInput(InputData& inputData, const SDL_KeyboardEvent& event, bool isDown) {
        if (event.repeat > 0)
            return;

        switch (event.keysym.scancode) {
            case SDL_SCANCODE_W:
                inputData.W = isDown;
                break;
            case SDL_SCANCODE_A:
                inputData.A = isDown;
                break;
            case SDL_SCANCODE_S:
                inputData.S = isDown;
                break;
            case SDL_SCANCODE_D:
                inputData.D = isDown;
                break;
            default:
                break;
        }
    }
    
    bool Input::PollInput() {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
                    ProcessKeyInput(s_InputData, event.key, true);
                break;
            case SDL_KEYUP:
                    ProcessKeyInput(s_InputData, event.key, false);
            default:
                break;
            }
        }
        
        return true;
    }
}