#pragma once
#include <cstdint>
#include <array>

struct ProcessorState
{
    std::array<std::uint8_t, 16> registersV;
    std::uint16_t registerI;

    std::uint16_t programCounter;
    std::uint8_t stackPointer;

    std::array<std::uint16_t, 16> stack;

    std::uint8_t delayTimer;
    std::uint8_t soundTimer;
};
