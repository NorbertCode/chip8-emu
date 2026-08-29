#pragma once
#include "debugger/debugWidget.hpp"
#include "memory/memory.hpp"
#include <functional>

namespace chip8::front
{
    class MemoryViewerWidget : public DebugWidget
    {
    public:
        MemoryViewerWidget(const core::Memory& memory, int bytesPerRow = 1);
        void render() override;

    private:
        int bytesPerRow = 0;
        std::reference_wrapper<const core::Memory> memory;
    };
}