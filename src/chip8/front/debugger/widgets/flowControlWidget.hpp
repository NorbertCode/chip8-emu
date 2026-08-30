#pragma once
#include "application.hpp"
#include "debugger/debugWidget.hpp"
#include <functional>

namespace chip8::front
{
    class FlowControlWidget : public DebugWidget
    {
    public:
        FlowControlWidget(Application& application, bool horizontal = false);
        void render() override;

    private:
        std::reference_wrapper<Application> application;
        bool horizontal = false;
    };
}