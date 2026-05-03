#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include "application/application.hpp"

const MemoryLayout layout = {
    .reserved_data = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    },
    .reserved_end = 0x200,
    .program_end = 0x1000,
    .reserved_read_only = true
};

constexpr Quirks quirks = {
    .vfReset = false,
    .indexIncrement = false,
    .displayClipping = false,
    .vyShifting = false,
    .vxJumping = false
};

constexpr ApplicationConfig config = {
    .loopFrequency = 600,
    .displayFrequency = 60
};

constexpr int displayWidth = 64;
constexpr int displayHeight = 32;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");

    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> file_buffer(size);
    file.read(reinterpret_cast<char*>(file_buffer.data()), size);

    file.close();

    Chip8 chip8(quirks, layout, displayWidth, displayHeight);
    chip8.getMemory().write_bytes(layout.reserved_end, file_buffer);

    Application application(chip8, config);
    application.run();

    SDL_Quit();

    return 0;
}