#include "processor.hpp"
#include <random>

void Processor::cls()
{
    display.clear();
}

void Processor::ret()
{
    programCounter = stack[stackPointer];
    stackPointer--;
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

void Processor::seRegByte(const std::uint8_t x, const std::uint8_t byte)
{
    programCounter += (registersV[x] == byte) * 2;
}

void Processor::sneRegByte(const std::uint8_t x, const std::uint8_t byte)
{
    programCounter += (registersV[x] != byte) * 2;
}

void Processor::seReg(const std::uint8_t x, const std::uint8_t y)
{
    programCounter += (registersV[x] == registersV[y]) * 2;
}

void Processor::ldRegByte(const std::uint8_t x, const std::uint8_t byte)
{
    registersV[x] = byte;
}

void Processor::addRegByte(const std::uint8_t x, const std::uint8_t byte)
{
    registersV[x] += byte;
}

void Processor::ldReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[x] = registersV[y];
}

void Processor::orReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[x] |= registersV[y];
}

void Processor::andReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[x] &= registersV[y];
}

void Processor::xorReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[x] ^= registersV[y];
}

void Processor::addReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[x] += registersV[y];
    registersV[0xF] = registersV[x] < registersV[y]; // Detect overflow by checking sum < operand
}

void Processor::subReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[0xF] = registersV[x] > registersV[y];
    registersV[x] -= registersV[y];
}

void Processor::shrReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[0xF] = registersV[x] & 0x1;
    registersV[x] >>= 1;
}

void Processor::subnReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[0xF] = registersV[y] > registersV[x];
    registersV[x] = registersV[y] - registersV[x];
}

void Processor::shlReg(const std::uint8_t x, const std::uint8_t y)
{
    registersV[0xF] = (registersV[x] & 0x80) >> 7;
    registersV[x] = registersV[x] << 1;
}

void Processor::sneReg(const std::uint8_t x, const std::uint8_t y)
{
    programCounter += (registersV[x] != registersV[y]) * 2;
}

void Processor::ldI(const std::uint16_t addr)
{
    registerI = addr;
}

void Processor::jpV0(const std::uint16_t addr)
{
    programCounter = registersV[0] + addr;
}

void Processor::rnd(const std::uint8_t x, const std::uint8_t byte)
{
    // TODO
}

void Processor::drw(const std::uint8_t x, const std::uint8_t y, const std::uint8_t nibble)
{
    const std::vector<std::uint8_t> sprite = memory.read_bytes(registerI, nibble);
    registersV[0xF] = display.xorSprite(registersV[x], registersV[y], sprite);
}

void Processor::skp(const std::uint8_t x)
{
    programCounter += keyboard.getKey(x) * 2;
}

void Processor::sknp(const std::uint8_t x)
{
    programCounter += !keyboard.getKey(x) * 2;
}

void Processor::ldRegDT(const std::uint8_t x)
{
    registersV[x] = delayTimer;
}

void Processor::ldK(const std::uint8_t x)
{
    const bool pressed = keyboard.getKey(x);

    if (pressed)
        registersV[x];
    else
        programCounter -= 2;
}

void Processor::ldDTReg(const std::uint8_t x)
{
    delayTimer = registersV[x];
}

void Processor::ldSTReg(const std::uint8_t x)
{
    soundTimer = registersV[x];
}

void Processor::addI(const std::uint8_t x)
{
    registerI += registersV[x];
}

void Processor::ldF(const std::uint8_t x)
{
    // TODO
}

void Processor::ldB(const std::uint8_t x)
{
    // TODO: Double Dabble Alg
}

void Processor::ldIReg(const std::uint8_t x)
{
    for (size_t registerIndex = 0, memoryIndex = registerI; registerIndex < x; ++registerIndex, ++memoryIndex)
        memory.write(memoryIndex, registersV[registerIndex]);
}

void Processor::ldRegI(const std::uint8_t x)
{
    for (size_t registerIndex = 0, memoryIndex = registerI; registerIndex < x; ++registerIndex, ++memoryIndex)
        registersV[registerIndex] = memory.read(memoryIndex);
}