#pragma once

#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine;
    
    struct InputData {
        bool W, A, S, D;
        Vector2 MousePos;
        bool LeftMouse, RightMouse, MiddleMouse;
    };
    
    class Input {
    public:
        Input(Engine* engine);
    public:
        bool PollInput();
    public:
        static const InputData& GetInput() { return s_InputData; }
    private:
        Engine* m_Engine;
        static InputData s_InputData;
    };
}
