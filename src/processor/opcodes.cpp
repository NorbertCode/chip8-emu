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
    std::uint8_t randomByte = static_cast<std::uint8_t>(uniformDistribution(random));
    registersV[x] = randomByte & byte;
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
    halted = true;

    auto onKeyPress = [this, x](const std::uint8_t keyCode) {
        registersV[x] = keyCode;
        halted = false;

        keyboard.clearOnKeyDown();
    };

    keyboard.setOnKeyPressed(onKeyPress);
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
    // Double Dabble Alg
    const int digit_bits = 12; // 8-bit value needs 12 bits for BCD representation
    const int digits = digit_bits / 4;

    std::uint32_t scratch_space = x;

    for (int i = 0; i < 8; ++i) // For every bit in x
    {
        for (unsigned int digit_bit = 8; digit_bit < 8 + digit_bits; digit_bit+=4) // Start at BCD representation offset
        {
            std::uint8_t digit = static_cast<std::uint8_t>(scratch_space >> digit_bit) & 0xF; 

            scratch_space += (digit >= 5) * (3U << digit_bit); // Add 3 to the correct BCD digit if its >= 5
        }

        scratch_space <<= 1;
    }

    for (int i = 0; i < digits; ++i)
    {
        std::uint8_t digit = static_cast<std::uint8_t>(scratch_space >> (8 + 4 * (digits - i - 1))) & 0xF;
        memory.write(registerI + i, digit);
    }
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