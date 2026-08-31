#pragma once
#include "debugger/debugWidget.hpp"
#include "memory/memory.hpp"
#include "disassembler.hpp"
#include "processor/processor.hpp"
#include "processor/quirks.hpp"
#include <functional>
#include <imgui.h>

namespace chip8::front
{
    class DisassemblyViewerWidget : public DebugWidget
    {
    public:
        DisassemblyViewerWidget(const core::Memory& memory, const core::Processor& processor);
        void render() override;

    private:
        std::reference_wrapper<const core::Memory> memory;
        std::reference_wrapper<const core::Processor> processor;

        disassembler::core::Disassembler disassembler;
        disassembler::core::DisassemblerConfig createDisassemblerConfig(const core::MemoryConfig& memoryConfig, const core::Quirks& quirks) const;

        const ImVec4 currentInstructionColor { 0.5f, 0.5f, 1.0f, 1.0f };
    };
}