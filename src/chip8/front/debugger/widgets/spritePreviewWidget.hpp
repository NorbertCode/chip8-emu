#pragma once
#include "debugger/debugWidget.hpp"
#include "memory/memory.hpp"
#include "processor/processor.hpp"
#include <concepts>
#include <functional>
#include <string>

namespace chip8::front
{
    class SpritePreviewWidget : public DebugWidget
    {
    public:
        SpritePreviewWidget(const core::Processor& processor, const core::Memory& memory);
        void render() override;

    private:
        std::reference_wrapper<const core::Processor> processor;
        std::reference_wrapper<const core::Memory> memory;

        bool hires = false;
        int height = 15;

        template <std::integral T>
        std::string getSpriteRow(T row)
        {
            std::string output;
            output.reserve(sizeof(row) * 8);

            for (int i = sizeof(row) * 8; i >= 0; --i)
            {
                bool isPixelOn = (row >> i) & 0x1;
                output += isPixelOn ? "#" : ".";
            }

            return output;
        }
    };
}