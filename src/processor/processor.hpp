#pragma once
#include <array>
#include "display/display.hpp"
#include "memory/memory.hpp"

class Processor
{
public:
    Processor(Memory& memory, Display& display, const std::uint16_t startProgramCounter);

    const std::uint16_t fetch();
    void execute(const std::uint16_t instruction);

private:
    std::array<std::uint8_t, 16> registersV;
    std::uint16_t registerI;

    std::uint16_t programCounter;
    std::uint8_t stackPointer;

    std::array<std::uint16_t, 16> stack;

    std::uint8_t delayTimer;
    std::uint8_t soundTimer;

    Memory memory;
    Display display;
};