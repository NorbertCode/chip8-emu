#pragma once
#include "debugger/debugWidget.hpp"
#include "debugger/debugger.hpp"
#include <functional>
#include <imgui.h>
#include <string>

namespace chip8::front
{
    class BreakpointsWidget : public DebugWidget
    {
    public:
        BreakpointsWidget(Debugger& debugger);
        void render() override;

    private:
        std::reference_wrapper<Debugger> debugger;

        std::string input = "0000";
        std::uint16_t line = 0;
    };
}