#pragma once
#include "memory/memory.hpp"
#include <cstdint>
#include <array>

class Processor
{
public:
    Processor(const std::uint16_t startProgramCounter) : programCounter(startProgramCounter) { }

    const std::uint16_t fetch(const Memory& memory);
    void execute(const std::uint16_t instruction);

private:
    std::array<std::uint8_t, 16> registersV;
    std::uint16_t registerI;

    std::uint16_t programCounter;
    std::uint8_t stackPointer;

    std::array<std::uint16_t, 16> stack;

    std::uint8_t delayTimer;
    std::uint8_t soundTimer;
};