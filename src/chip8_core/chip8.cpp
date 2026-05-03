#include "chip8.hpp"

Chip8::Chip8(const Quirks& quirks, const MemoryLayout& layout, std::uint8_t displayWidth, std::uint8_t displayHeight)
    : memory(layout), display(displayWidth, displayHeight), processor(memory, display, keyboard, quirks, layout.reserved_end) { }