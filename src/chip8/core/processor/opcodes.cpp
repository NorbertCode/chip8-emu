#include "processor.hpp"
#include <random>

namespace chip8::core
{
    void Processor::scrollDown(std::uint8_t pixels)
    {
        display.get().scrollDown(pixels, quirks.loresWholePixelScrolling);
    }

    void Processor::cls()
    {
        display.get().clear();
    }

    void Processor::ret()
    {
        if (stackPointer <= 0)
            stackPointer = stack.size();

        programCounter = stack[--stackPointer];
    }

    void Processor::scrollRight()
    {
        display.get().scrollRight(4, quirks.loresWholePixelScrolling);
    }

    void Processor::scrollLeft()
    {
        display.get().scrollLeft(4, quirks.loresWholePixelScrolling);
    }

    void Processor::exit()
    {
        running = false;
    }

    void Processor::setDisplayMode(ResolutionMode mode)
    {
        if (display.get().getResolutionMode() == mode)
            return;

        if (quirks.clearOnDisplayModeChange)
            display.get().clear();

        display.get().setResolutionMode(mode);
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
        std::uint8_t flag = registersV[x] >= registersV[y];

        registersV[x] -= registersV[y];
        registersV[0xF] = flag;
    }

    void Processor::shrReg(std::uint8_t x, std::uint8_t y)
    {
        std::uint8_t value = quirks.vyShifting ? registersV[y] : registersV[x];

        registersV[x] = value >> 1;
        registersV[0xF] = value & 0x1;
    }

    void Processor::subnReg(std::uint8_t x, std::uint8_t y)
    {
        std::uint8_t flag = registersV[y] >= registersV[x];

        registersV[x] = registersV[y] - registersV[x];
        registersV[0xF] = flag;
    }

    void Processor::shlReg(std::uint8_t x, std::uint8_t y)
    {
        std::uint8_t value = quirks.vyShifting ? registersV[y] : registersV[x];

        registersV[x] = value << 1;
        registersV[0xF] = (value & 0x80) >> 7;
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
        std::vector<std::uint8_t> sprite = memory.get().read_bytes(registerI, nibble);
        int collisions = display.get().xorSprite(registersV[x], registersV[y], sprite, quirks.displayClipping);
        registersV[0xF] = static_cast<std::uint8_t>(quirks.vfCollisionCounter && display.get().getResolutionMode() == ResolutionMode::Hires ? collisions : collisions > 0);

        if (quirks.waitForVBlank)
            waitingForVBlank = true;
    }

    void Processor::drwHires(std::uint8_t x, std::uint8_t y)
    {
        std::vector<std::uint8_t> sprite = memory.get().read_bytes(registerI, 32);

        if (display.get().getResolutionMode() == ResolutionMode::Hires || quirks.loresSpriteHandling == LoresSpriteHandling::DrawWide)
        {
            int collisions = display.get().xorHiresSprite(registersV[x], registersV[y], sprite, quirks.displayClipping);
            registersV[0xF] = static_cast<std::uint8_t>(quirks.vfCollisionCounter && display.get().getResolutionMode() == ResolutionMode::Hires ? collisions : collisions > 0);

            if (quirks.waitForVBlank)
                waitingForVBlank = true;
        }
        else if (quirks.loresSpriteHandling == LoresSpriteHandling::DrawTall)
            drw(x, y, 16);
    }

    void Processor::skp(std::uint8_t x)
    {
        programCounter += keyboard.get().getKey(registersV[x]) * 2;
    }

    void Processor::sknp(std::uint8_t x)
    {
        programCounter += !keyboard.get().getKey(registersV[x]) * 2;
    }

    void Processor::ldRegDT(std::uint8_t x)
    {
        registersV[x] = delayTimer;
    }

    void Processor::ldK(std::uint8_t x)
    {
        waitingForKeypress = true;

        auto onKeyPress = [this, x](std::uint8_t keyCode) {
            registersV[x] = keyCode;
            waitingForKeypress = false;

            keyboard.get().clearOnKeyPressed();
        };

        keyboard.get().setOnKeyPressed(onKeyPress);
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

    void Processor::ldFHires(std::uint8_t x)
    {
        registerI = (registersV[x] * 10) + 80; // 80 because there are 16 lores characters, each 5 bytes long
    }

    void Processor::ldB(std::uint8_t x)
    {
        // 8-bit numbers are at most 3-digit
        memory.get().write(registerI, registersV[x] / 100);
        memory.get().write(registerI + 1, (registersV[x] / 10) % 10);
        memory.get().write(registerI + 2, registersV[x] % 10);
    }

    void Processor::ldIReg(std::uint8_t x)
    {
        std::uint16_t memoryIndex = registerI;

        for (size_t registerIndex = 0; registerIndex <= x; ++registerIndex, ++memoryIndex)
            memory.get().write(memoryIndex, registersV[registerIndex]);

        if (quirks.indexIncrement)
            registerI = memoryIndex;
    }

    void Processor::ldRegI(std::uint8_t x)
    {
        std::uint16_t memoryIndex = registerI;

        for (size_t registerIndex = 0; registerIndex <= x; ++registerIndex, ++memoryIndex)
            registersV[registerIndex] = memory.get().read(memoryIndex);

        if (quirks.indexIncrement)
            registerI = memoryIndex;
    }

    void Processor::ldRplReg(std::uint8_t x)
    {
        storage.get().write(std::vector<std::uint8_t>(registersV.begin(), registersV.begin() + x + 1));
    }

    void Processor::ldRegRpl(std::uint8_t x)
    {
        const auto& data = storage.get().read();
        std::copy(data.begin(), data.begin() + x + 1, registersV.begin());
    }
}