#pragma once

#include "maths/Math.hpp"

namespace Phezu {
    
    class Engine;
    
    struct InputData {
        bool W, A, S, D;
        bool Up, Right, Down, Left;
        bool Space, F, G, H;
        Vector2 MousePos;
        bool LeftMouse, RightMouse, MiddleMouse;
    };
    
    class Input {
    public:
        Input(Engine* engine);
        ~Input();
    public:
        bool PollInput();
    public:
        static const InputData& GetInput() { return s_InputData; }
    private:
        Engine* m_Engine;
        static InputData s_InputData;
    };
}
