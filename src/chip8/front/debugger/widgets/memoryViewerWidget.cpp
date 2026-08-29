#include "memoryViewerWidget.hpp"
#include "memory/memory.hpp"
#include <imgui.h>

namespace chip8::front
{
    MemoryViewerWidget::MemoryViewerWidget(const chip8::core::Memory& memory, int bytesPerRow)
        : memory(memory),
        bytesPerRow(bytesPerRow) { }

    void MemoryViewerWidget::render()
    {
        ImGui::Begin("Memory Viewer");

        int bytesToRead = memory.get().getMemoryConfig().programEnd;
        int totalRows = (bytesToRead + bytesPerRow - 1) / bytesPerRow;

        ImGuiListClipper clipper;
        clipper.Begin(totalRows);

        while (clipper.Step())
        {
            for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
            {
                int baseAddress = row * bytesPerRow;

                ImGui::Text("0x%04X: ", baseAddress);
                ImGui::SameLine();

                for (int byte = 0; byte < bytesPerRow; ++byte)
                {
                    int address = baseAddress + byte;

                    if (address < bytesToRead)
                        ImGui::Text("0x%02X", memory.get().read(address));

                    if (byte < bytesPerRow - 1)
                        ImGui::SameLine();
                }
            }
        }

        ImGui::End();
    }
}