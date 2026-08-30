#pragma once
#include "application.hpp"
#include "debugger/debugWidget.hpp"
#include <functional>
#include <imgui.h>

namespace chip8::front
{
    class BreakpointsWidget : public DebugWidget
    {
    public:
        BreakpointsWidget(Application& application);
        void render() override;

    private:
        std::reference_wrapper<Application> application;

        ImU16 line = 0;
    };
}