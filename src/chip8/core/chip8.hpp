#pragma once
#include "processor/processor.hpp"
#include "memory/memory.hpp"
#include "memory/storage.hpp"
#include "peripherals/display.hpp"
#include "peripherals/keyboard.hpp"

namespace chip8::core
{
    class Chip8
    {
    public:
        Chip8(const Quirks& quirks, const MemoryConfig& memoryConfig, const DisplayConfig& displayConfig, std::function<void(std::span<const std::uint8_t, 16>)> onStorageWriteCallback);

        Processor& getProcessor() { return processor; }
        Memory& getMemory() { return memory; }
        Storage& getStorage() { return storage; }
        Display& getDisplay() { return display; }
        Keyboard& getKeyboard() { return keyboard; }

        void loadRom(std::span<const std::uint8_t> rom);

    private:
        Memory memory;
        Storage storage;
        Display display;
        Keyboard keyboard;
        Processor processor;
    };
}