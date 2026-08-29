#pragma once
#include "debugger/debugWidget.hpp"

namespace chip8::front
{
    class DemoWidget : public DebugWidget
    {
    public:
        void render() override;
    };
}