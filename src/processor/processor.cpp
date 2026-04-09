#include "processor.hpp"

const std::uint16_t Processor::fetch(const Memory& memory)
{
    const std::uint16_t instruction = (memory.read(programCounter) << 8) | memory.read(programCounter + 1);

    programCounter += 2;

    return instruction;
}

void Processor::execute(const std::uint16_t instruction)
{
    const std::array<std::uint8_t, 4> nibbles = {
        (instruction & 0xF000) >> 12,
        (instruction & 0x0F00) >> 8,
        (instruction & 0x00F0) >> 4,
        (instruction & 0x000F)
    };

    const std::uint16_t addr = instruction & 0xFFF;
    const std::uint8_t byte = instruction & 0xFF;

    switch (nibbles[0])
    {
        case 0x0: 
            switch (instruction)
            {
                case 0x00E0: break; // CLS
                case 0x00EE: break; // RET
            }
            break;
        case 0x1: break;
        case 0x2: break;
        case 0x3: break;
        case 0x4: break;
        case 0x5: break;
        case 0x6: break;
        case 0x7: break;
        case 0x8: 
            switch (nibbles[3])
            {
                case 0x0: break; // LD Vx, Vy
                case 0x1: break; // OR Vx, Vy
                case 0x2: break; // AND Vx, Vy
                case 0x3: break; // XOR Vx, Vy
                case 0x4: break; // ADD Vx, Vy
                case 0x5: break; // SUB Vx, Vy
                case 0x6: break; // SHR Vxm {, Vy}
                case 0x7: break; // SUBN Vx, Vy
                case 0xE: break; // SHL Vx, {, Vy}
            }
            break;
        case 0x9: break;
        case 0xA: break;
        case 0xB: break;
        case 0xC: break;
        case 0xD: break;
        case 0xE:
            switch (byte)
            {
                case 0x9E: break; // SKP Vx
                case 0xA1: break; // SKNP Vx
            }
            break;
        case 0xF: break;
            switch (byte)
            {
                case 0x07: break; // LD Vx, DT
                case 0x0A: break; // LD Vx, byte
                case 0x15: break; // LD DT, Vx
                case 0x18: break; // LD ST, Vx
                case 0x1E: break; // ADD I, Vx
                case 0x29: break; // LD F, Vx
                case 0x33: break; // LD B, Vx
                case 0x55: break; // LD [I], Vx
                case 0x65: break; // LD Vx, I
            }
            break;
    }
}