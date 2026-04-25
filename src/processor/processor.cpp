#include "processor.hpp"
#include "opcodes.cpp"

Processor::Processor(Memory& memory, Display& display, const std::uint16_t startProgramCounter)
    : memory(memory), display(display)
{
    programCounter = startProgramCounter;
}

std::uint16_t Processor::fetch()
{
    const std::uint16_t instruction = (memory.read(programCounter) << 8) | memory.read(programCounter + 1);

    programCounter += 2;

    return instruction;
}

void Processor::execute(const std::uint16_t instruction)
{
    const std::array<std::uint8_t, 4> nibbles = {
        static_cast<std::uint8_t>((instruction & 0xF000) >> 12),
        static_cast<std::uint8_t>((instruction & 0x0F00) >> 8),
        static_cast<std::uint8_t>((instruction & 0x00F0) >> 4),
        static_cast<std::uint8_t>(instruction & 0x000F)
    };

    const std::uint8_t x = nibbles[1];
    const std::uint8_t y = nibbles[2];
    const std::uint16_t addr = instruction & 0xFFF;
    const std::uint8_t byte = instruction & 0xFF;

    switch (nibbles[0])
    {
        case 0x0: 
            switch (instruction)
            {
                case 0x00E0: cls(); break; // CLS
                case 0x00EE: ret(); break; // RET
            }
            break;
        case 0x1: jp(addr); break;            // JP
        case 0x2: call(addr); break;          // CALL
        case 0x3: seRegByte(x, byte); break;  // SE Vx, byte
        case 0x4: sneRegByte(x, byte); break; // SNE Vx, byte
        case 0x5: seReg(x, y); break;         // SE Vx, Vy
        case 0x6: ldRegByte(x, byte); break;  // LD Vx, byte
        case 0x7: addRegByte(x, byte); break; // ADD Vx, byte
        case 0x8: 
            switch (nibbles[3])
            {
                case 0x0: ldReg(x, y); break;   // LD Vx, Vy
                case 0x1: orReg(x, y); break;   // OR Vx, Vy
                case 0x2: andReg(x, y); break;  // AND Vx, Vy
                case 0x3: xorReg(x, y); break;  // XOR Vx, Vy
                case 0x4: addReg(x, y); break;  // ADD Vx, Vy
                case 0x5: subReg(x, y); break;  // SUB Vx, Vy
                case 0x6: shrReg(x, y); break;  // SHR Vxm {, Vy}
                case 0x7: subnReg(x, y); break; // SUBN Vx, Vy
                case 0xE: shlReg(x, y); break;  // SHL Vx, {, Vy}
            }
            break;
        case 0x9: sneReg(x, y); break;          // SNE Vx, Vy
        case 0xA: ldI(addr); break;             // LD I, addr
        case 0xB: jpV0(addr); break;            // JP V0, addr
        case 0xC: rnd(x, byte); break;          // RND Vx, byte
        case 0xD: drw(x, y, nibbles[3]); break; // DRW Vx, Vy, nibble
        case 0xE:
            switch (byte)
            {
                case 0x9E: skp(x); break;  // SKP Vx
                case 0xA1: sknp(x); break; // SKNP Vx
            }
            break;
        case 0xF: break;
            switch (byte)
            {
                case 0x07: ldRegDT(x); break; // LD Vx, DT
                case 0x0A: ldK(x); break;     // LD Vx, byte
                case 0x15: ldDTReg(x); break; // LD DT, Vx
                case 0x18: ldSTReg(x); break; // LD ST, Vx
                case 0x1E: addI(x); break;    // ADD I, Vx
                case 0x29: ldF(x); break;     // LD F, Vx
                case 0x33: ldB(x); break;     // LD B, Vx
                case 0x55: ldIReg(x); break;  // LD [I], Vx
                case 0x65: ldRegI(x); break;  // LD Vx, I
            }
            break;
    }
}