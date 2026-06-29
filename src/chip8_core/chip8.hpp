#pragma once
#include "processor/processor.hpp"
#include "memory/memory.hpp"
#include "peripherals/display.hpp"
#include "peripherals/keyboard.hpp"

class Chip8
{
public:
    Chip8(const Quirks& quirks, const MemoryConfig& memoryConfig, std::uint8_t displayWidth, std::uint8_t displayHeight);

    Processor& getProcessor() { return processor; }
    Memory& getMemory() { return memory; }
    Display& getDisplay() { return display; }
    Keyboard& getKeyboard() { return keyboard; }

private:
    Processor processor;
    Memory memory;
    Display display;
    Keyboard keyboard;
};