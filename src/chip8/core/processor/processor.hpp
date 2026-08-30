#pragma once
#include <array>
#include <random>
#include "quirks.hpp"
#include "memory/memory.hpp"
#include "memory/storage.hpp"
#include "peripherals/display.hpp"
#include "peripherals/keyboard.hpp"

namespace chip8::core
{
    class Processor
    {
    public:
        Processor(Memory& memory, Storage& storage, Display& display, Keyboard& keyboard, Quirks quirks, std::uint16_t startProgramCounter);

        void step();
        void tickTimers();

        std::uint16_t fetch();
        void execute(std::uint16_t instruction);

        std::span<const std::uint8_t, 16> getRegistersV() const { return registersV; }
        std::uint16_t getRegisterI() const { return registerI; }

        std::uint16_t getProgramCounter() const { return programCounter; }
        std::uint8_t getStackPointer() const { return stackPointer; }

        std::span<const std::uint16_t, 16> getStack() const { return stack; }

        std::uint8_t getDelayTimer() const { return delayTimer; }
        std::uint8_t getSoundTimer() const { return soundTimer; }

        const Quirks& getQuirks() const { return quirks; }

        bool isRunning() const { return running; }
        bool isHalted() const { return halted; }

        void reset();

    private:
        std::array<std::uint8_t, 16> registersV{};
        std::uint16_t registerI = 0;

        std::uint16_t programCounter = 0;
        std::uint8_t stackPointer = 0;

        std::array<std::uint16_t, 16> stack{};

        std::uint8_t delayTimer = 0;
        std::uint8_t soundTimer = 0;

        bool running = true;
        bool halted = false;

        std::reference_wrapper<Memory> memory;
        std::reference_wrapper<Storage> storage;
        std::reference_wrapper<Display> display;
        std::reference_wrapper<Keyboard> keyboard;

        Quirks quirks;

        std::mt19937 random;
        std::uniform_int_distribution<std::uint16_t> uniformDistribution;

        void scrollDown(std::uint8_t pixels); // Scroll display down by n pixels
        void cls(); // Clear display
        void ret(); // Return
        void scrollRight(); // Scroll display right by 4 pixels
        void scrollLeft(); // Scroll display left by 4 pixels
        void exit(); // Exit the interpreter
        void setDisplayMode(ResolutionMode mode); // Switch display mode
        void jp(std::uint16_t addr); // Jump to addr
        void call(std::uint16_t addr); // Call addr
        void seRegByte(std::uint8_t x, std::uint8_t byte); // Skip next instruction if Vx == byte
        void sneRegByte(std::uint8_t x, std::uint8_t byte); // Skip next instruction if Vx != byte
        void seReg(std::uint8_t x, std::uint8_t y); // Skip next instruction if Vx == Vy
        void ldRegByte(std::uint8_t x, std::uint8_t byte); // Vx = byte
        void addRegByte(std::uint8_t x, std::uint8_t byte); // Vx = Vx + byte
        void ldReg(std::uint8_t x, std::uint8_t y); // Vx = Vy
        void orReg(std::uint8_t x, std::uint8_t y); // Vx = Vx | Vy
        void andReg(std::uint8_t x, std::uint8_t y); // Vx = Vx & Vy
        void xorReg(std::uint8_t x, std::uint8_t y); // Vx = Vx ^ Vy
        void addReg(std::uint8_t x, std::uint8_t y); // Vx = Vx + Vy, VF = carry
        void subReg(std::uint8_t x, std::uint8_t y); // Vx = Vx - Vy, VF = !borrow
        void shrReg(std::uint8_t x, std::uint8_t y); // Vx = Vx >> 1, VF = LSb(Vx)
        void subnReg(std::uint8_t x, std::uint8_t y); // Vx = Vy - Vx, VF = !borrow
        void shlReg(std::uint8_t x, std::uint8_t y); // Vx = Vx << 1, VF = MSb(Vx)
        void sneReg(std::uint8_t x, std::uint8_t y); // Skip next instruction if Vx != Vy
        void ldI(std::uint16_t addr); // I = addr
        void jpV0(std::uint16_t addr); // Jump to V0 + addr
        void rnd(std::uint8_t x, std::uint8_t byte); // Vx = random() & byte
        void drw(std::uint8_t x, std::uint8_t y, std::uint8_t nibble); // Draw n-byte sprite from I at (Vx, Vy), VF = collision
        void drwHires(std::uint8_t x, std::uint8_t y); // Draw hires sprite from I at (Vx, Vy), VF = collision
        void skp(std::uint8_t x); // Skip next instruction if key with value Vx is pressed
        void sknp(std::uint8_t x); // Skip next instruction if key with value Vx is not pressed
        void ldRegDT(std::uint8_t x); // Vx = DT
        void ldK(std::uint8_t x); // Wait until key press, then Vx = key
        void ldDTReg(std::uint8_t x); // DT = Vx
        void ldSTReg(std::uint8_t x); // ST = Vx
        void addI(std::uint8_t x); // I = I + Vx
        void ldF(std::uint8_t x); // I = location of sprite for digit Vx
        void ldFHires(std::uint8_t x); // I = location of hires sprite for digit Vx
        void ldB(std::uint8_t x); // Store BCD representation of Vx at (I, I + 1, I + 2)
        void ldIReg(std::uint8_t x); // Store registers V0-Vx in memory starting at address I
        void ldRegI(std::uint8_t x); // Read registers V0-Vx from memory starting at address I
        void ldRplReg(std::uint8_t x); // Store registers V0-Vx in storage
        void ldRegRpl(std::uint8_t x); // Read registers V0-Vx from storage
    };
}