#pragma once
#include "debugger/debugWidget.hpp"
#include "memory/memory.hpp"
#include "disassembler.hpp"
#include "processor/quirks.hpp"
#include <functional>

namespace chip8::front
{
    class DisassemblyViewerWidget : public DebugWidget
    {
    public:
        DisassemblyViewerWidget(const core::Memory& memory, const core::Quirks& quirks);
        void render() override;

    private:
        std::reference_wrapper<const core::Memory> memory;
        disassembler::core::Disassembler disassembler;

        disassembler::core::DisassemblerConfig createDisassemblerConfig(const core::MemoryConfig& memoryConfig, const core::Quirks& quirks) const;
    };
}