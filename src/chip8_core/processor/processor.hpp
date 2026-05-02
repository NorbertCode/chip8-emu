#pragma once
#include <array>
#include <random>
#include "memory/memory.hpp"
#include "peripherals/display.hpp"
#include "peripherals/keyboard.hpp"

struct Quirks
{
    bool vfReset;
    bool indexIncrement;
    bool displayClipping;
    bool vyShifting;
    bool vxJumping;
};

class Processor
{
public:
    Processor(Memory& memory, Display& display, Keyboard& keyboard, const Quirks& quirks, std::uint16_t startProgramCounter);

    void step();
    void tick_timers();

    std::uint16_t fetch();
    void execute(std::uint16_t instruction);

    const std::array<std::uint8_t, 16>& getRegistersV() const { return registersV; }
    std::uint16_t getRegisterI() const { return registerI; }

    std::uint16_t getProgramCounter() const { return programCounter; }
    std::uint8_t getStackPointer() const { return stackPointer; }

    const std::array<std::uint16_t, 16>& getStack() const { return stack; }

    std::uint8_t getDelayTimer() const { return delayTimer; }
    std::uint8_t getSoundTimer() const { return soundTimer; }

    bool isHalted() const { return halted; }

private:
    std::array<std::uint8_t, 16> registersV{};
    std::uint16_t registerI = 0;

    std::uint16_t programCounter = 0;
    std::uint8_t stackPointer = 0;

    std::array<std::uint16_t, 16> stack{};

    std::uint8_t delayTimer = 0;
    std::uint8_t soundTimer = 0;

    bool halted = false;

    Memory& memory;
    Display& display;
    Keyboard& keyboard;

    const Quirks& quirks;

    std::mt19937 random;
    std::uniform_int_distribution<std::uint16_t> uniformDistribution;

    void cls(); // Clear display
    void ret(); // Return
    void jp(const std::uint16_t addr); // Jump to addr
    void call(const std::uint16_t addr); // Call addr
    void seRegByte(const std::uint8_t x, const std::uint8_t byte); // Skip next instruction if Vx == byte
    void sneRegByte(const std::uint8_t x, const std::uint8_t byte); // Skip next instruction if Vx != byte
    void seReg(const std::uint8_t x, const std::uint8_t y); // Skip next instruction if Vx == Vy
    void ldRegByte(const std::uint8_t x, const std::uint8_t byte); // Vx = byte
    void addRegByte(const std::uint8_t x, const std::uint8_t byte); // Vx = Vx + byte
    void ldReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vy
    void orReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx | Vy
    void andReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx & Vy
    void xorReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx ^ Vy
    void addReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx + Vy, VF = carry
    void subReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx - Vy, VF = !borrow
    void shrReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx >> 1, VF = LSb(Vx)
    void subnReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vy - Vx, VF = !borrow
    void shlReg(const std::uint8_t x, const std::uint8_t y); // Vx = Vx << 1, VF = MSb(Vx)
    void sneReg(const std::uint8_t x, const std::uint8_t y); // Skip next instruction if Vx != Vy
    void ldI(const std::uint16_t addr); // I = addr
    void jpV0(const std::uint16_t addr); // Jump to V0 + addr
    void rnd(const std::uint8_t x, const std::uint8_t byte); // Vx = random() & byte
    void drw(const std::uint8_t x, const std::uint8_t y, const std::uint8_t nibble); // Draw n-byte sprite from I at (Vx, Vy), VF = collision
    void skp(const std::uint8_t x); // Skip next instruction if key with value Vx is pressed
    void sknp(const std::uint8_t x); // Skip next instruction if key with value Vx is not pressed
    void ldRegDT(const std::uint8_t x); // Vx = DT
    void ldK(const std::uint8_t x); // Wait until key press, then Vx = key
    void ldDTReg(const std::uint8_t x); // DT = Vx
    void ldSTReg(const std::uint8_t x); // ST = Vx
    void addI(const std::uint8_t x); // I = I + Vx
    void ldF(const std::uint8_t x); // I = location of sprite for digit Vx
    void ldB(const std::uint8_t x); // Store BCD representation of Vx at (I, I + 1, I + 2)
    void ldIReg(const std::uint8_t x); // Store registers V0-Vx in memory starting at address I
    void ldRegI(const std::uint8_t x); // Read registers V0-Vx from memory starting at address I
};