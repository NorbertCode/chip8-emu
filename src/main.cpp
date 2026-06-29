#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include "application/application.hpp"

const MemoryConfig memoryConfig = {
    .reservedEnd = 0x200,
    .programEnd = 0x1000,
    .reservedReadOnly = true
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
    .timerFrequency = 60,
    .displayFrequency = 60,
    .audioFrequency = 440,
    .keyMap = { "X", "1", "2", "3", "Q", "W", "E", "A", "S", "D", "Z", "C", "4", "R", "F", "V" }
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

    Chip8 chip8(quirks, memoryConfig, displayWidth, displayHeight);
    chip8.getMemory().write_bytes(memoryConfig.reservedEnd, file_buffer);

    Application application(chip8, config);
    application.run();

    SDL_Quit();

    return 0;
}