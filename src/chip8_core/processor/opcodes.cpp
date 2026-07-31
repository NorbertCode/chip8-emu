#include "processor.hpp"
#include <random>

void Processor::cls()
{
    display.clear();
}

void Processor::ret()
{
    programCounter = stack[--stackPointer];
}

void Processor::exit()
{
    running = false;
}

void Processor::setDisplayMode(ResolutionMode mode)
{
    if (display.getResolutionMode() == mode)
        return;

    if (quirks.clearOnDisplayModeChange)
        display.clear();

    display.setResolutionMode(mode);
}

void Processor::jp(std::uint16_t addr)
{
    programCounter = addr;
}

void Processor::call(std::uint16_t addr)
{
    stack[stackPointer++] = programCounter;
    programCounter = addr;
}

void Processor::seRegByte(std::uint8_t x, std::uint8_t byte)
{
    programCounter += (registersV[x] == byte) * 2;
}

void Processor::sneRegByte(std::uint8_t x, std::uint8_t byte)
{
    programCounter += (registersV[x] != byte) * 2;
}

void Processor::seReg(std::uint8_t x, std::uint8_t y)
{
    programCounter += (registersV[x] == registersV[y]) * 2;
}

void Processor::ldRegByte(std::uint8_t x, std::uint8_t byte)
{
    registersV[x] = byte;
}

void Processor::addRegByte(std::uint8_t x, std::uint8_t byte)
{
    registersV[x] += byte;
}

void Processor::ldReg(std::uint8_t x, std::uint8_t y)
{
    registersV[x] = registersV[y];
}

void Processor::orReg(std::uint8_t x, std::uint8_t y)
{
    registersV[x] |= registersV[y];

    if (quirks.vfReset)
        registersV[0xF] = 0;
}

void Processor::andReg(std::uint8_t x, std::uint8_t y)
{
    registersV[x] &= registersV[y];

    if (quirks.vfReset)
        registersV[0xF] = 0;
}

void Processor::xorReg(std::uint8_t x, std::uint8_t y)
{
    registersV[x] ^= registersV[y];

    if (quirks.vfReset)
        registersV[0xF] = 0;
}

void Processor::addReg(std::uint8_t x, std::uint8_t y)
{
    registersV[x] += registersV[y];
    registersV[0xF] = registersV[x] < registersV[y]; // Detect overflow by checking sum < operand
}

void Processor::subReg(std::uint8_t x, std::uint8_t y)
{
    registersV[0xF] = registersV[x] > registersV[y];
    registersV[x] -= registersV[y];
}

void Processor::shrReg(std::uint8_t x, std::uint8_t y)
{
    std::uint8_t value = quirks.vyShifting ? registersV[y] : registersV[x];

    registersV[0xF] = value & 0x1;
    registersV[x] = value >> 1;
}

void Processor::subnReg(std::uint8_t x, std::uint8_t y)
{
    registersV[0xF] = registersV[y] > registersV[x];
    registersV[x] = registersV[y] - registersV[x];
}

void Processor::shlReg(std::uint8_t x, std::uint8_t y)
{
    std::uint8_t value = quirks.vyShifting ? registersV[y] : registersV[x];

    registersV[0xF] = (value & 0x80) >> 7;
    registersV[x] = value << 1;
}

void Processor::sneReg(std::uint8_t x, std::uint8_t y)
{
    programCounter += (registersV[x] != registersV[y]) * 2;
}

void Processor::ldI(std::uint16_t addr)
{
    registerI = addr;
}

void Processor::jpV0(std::uint16_t addr)
{
    std::uint8_t registerAddress = quirks.vxJumping ? registersV[(addr >> 8) & 0xF] : registersV[0x0];
    programCounter = registerAddress + addr;
}

void Processor::rnd(std::uint8_t x, std::uint8_t byte)
{
    std::uint8_t randomByte = static_cast<std::uint8_t>(uniformDistribution(random));
    registersV[x] = randomByte & byte;
}

void Processor::drw(std::uint8_t x, std::uint8_t y, std::uint8_t nibble)
{
    std::vector<std::uint8_t> sprite = memory.read_bytes(registerI, nibble);
    registersV[0xF] = display.xorSprite(registersV[x], registersV[y], sprite, quirks.displayClipping);
}

void Processor::skp(std::uint8_t x)
{
    programCounter += keyboard.getKey(registersV[x]) * 2;
}

void Processor::sknp(std::uint8_t x)
{
    programCounter += !keyboard.getKey(registersV[x]) * 2;
}

void Processor::ldRegDT(std::uint8_t x)
{
    registersV[x] = delayTimer;
}

void Processor::ldK(std::uint8_t x)
{
    halted = true;

    auto onKeyPress = [this, x](std::uint8_t keyCode) {
        registersV[x] = keyCode;
        halted = false;

        keyboard.clearOnKeyPressed();
    };

    keyboard.setOnKeyPressed(onKeyPress);
}

void Processor::ldDTReg(std::uint8_t x)
{
    delayTimer = registersV[x];
}

void Processor::ldSTReg(std::uint8_t x)
{
    soundTimer = registersV[x];
}

void Processor::addI(std::uint8_t x)
{
    registerI += registersV[x];
}

void Processor::ldF(std::uint8_t x)
{
    registerI = registersV[x] * 5; // Each character is 5 bytes long
}

void Processor::ldB(std::uint8_t x)
{
    // 8-bit numbers are at most 3-digit
    memory.write(registerI, registersV[x] / 100);
    memory.write(registerI + 1, (registersV[x] / 10) % 10);
    memory.write(registerI + 2, registersV[x] % 10);
}

void Processor::ldIReg(std::uint8_t x)
{
    size_t registerIndex, memoryIndex;

    for (registerIndex = 0, memoryIndex = registerI; registerIndex <= x; ++registerIndex, ++memoryIndex)
        memory.write(memoryIndex, registersV[registerIndex]);

    if (quirks.indexIncrement)
        registerI = memoryIndex + 1;
}

void Processor::ldRegI(std::uint8_t x)
{
    size_t registerIndex, memoryIndex;

    for (registerIndex = 0, memoryIndex = registerI; registerIndex <= x; ++registerIndex, ++memoryIndex)
        registersV[registerIndex] = memory.read(memoryIndex);

    if (quirks.indexIncrement)
        registerI = memoryIndex + 1;
}