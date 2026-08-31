#pragma once
#include "debugger/debugWidget.hpp"
#include "processor/processor.hpp"
#include <functional>

namespace chip8::front
{
    class StackViewerWidget : public DebugWidget
    {
    public:
        StackViewerWidget(const core::Processor& processor, bool horizontal = false);
        void render() override;

    private:
        std::reference_wrapper<const core::Processor> processor;

        bool horizontal = false;
        float spacing = 20.0f;
    };
}