#include "disassemblyViewerWidget.hpp"
#include "disassembler.hpp"
#include "memory/memory.hpp"
#include "processor/processor.hpp"
#include "processor/quirks.hpp"
#include <imgui.h>

namespace chip8::front
{
    DisassemblyViewerWidget::DisassemblyViewerWidget(const chip8::core::Memory& memory, const core::Processor& processor)
        : memory(memory),
        processor(processor),
        disassembler(createDisassemblerConfig(memory.getMemoryConfig(), processor.getQuirks())) { }

    void DisassemblyViewerWidget::render()
    {
        ImGui::Begin("Disassembly");

        int programStart = memory.get().getMemoryConfig().reservedEnd;
        int bytesToRead = memory.get().getMemoryConfig().programEnd;
        int totalRows = (bytesToRead - programStart + 1) / 2;

        // Update config in case lores sprite handling was updated
        disassembler.getConfig().hiresOperations = (processor.get().getQuirks().loresSpriteHandling != core::LoresSpriteHandling::NoOperation);

        ImGuiListClipper clipper;
        clipper.Begin(totalRows);

        while (clipper.Step())
        {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
            {
                int address = programStart + (row * 2);
                std::uint16_t instruction = (static_cast<std::uint16_t>(memory.get().read(address)) << 8) + memory.get().read(address + 1);

                if (address == processor.get().getProgramCounter())
                    ImGui::TextColored(currentInstructionColor, "0x%04X: %s", address, disassembler.disassemble(instruction).c_str());
                else
                    ImGui::Text("0x%04X: %s", address, disassembler.disassemble(instruction).c_str());
            }
        }

        ImGui::End();
    }

    disassembler::core::DisassemblerConfig DisassemblyViewerWidget::createDisassemblerConfig(const core::MemoryConfig& memoryConfig, const core::Quirks& quirks) const
    {
        return {
            .programStart = memoryConfig.reservedEnd,
            .vyShifting = quirks.vyShifting,
            .vxJumping = quirks.vxJumping,
            .hiresOperations = quirks.loresSpriteHandling != core::LoresSpriteHandling::NoOperation
        };
    }
}