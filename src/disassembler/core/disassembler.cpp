#include "disassembler.hpp"
#include <format>

namespace disassembler::core
{
    Disassembler::Disassembler(DisassemblerConfig config)
        : config(config) { }

    std::uint16_t Disassembler::getProgramStart() const
    {
        return config.programStart;
    }

    std::string Disassembler::disassemble(std::uint16_t instruction)
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
                    case 0x00E0: return "CLS";
                    case 0x00EE: return "RET";
                    case 0x00FB: 
                        if (config.hiresOperations) 
                            return "SCR";
                        break;
                    case 0x00FC: 
                        if (config.hiresOperations) 
                            return "SCL";
                        break;
                    case 0x00FD: return "EXIT";
                    case 0x00FE: 
                        if (config.hiresOperations) 
                            return "LOW";
                        break;
                    case 0x00FF: 
                        if (config.hiresOperations) 
                            return "HIGH";
                        break;
                    default: 
                        if (nibbles[2] == 0xC && config.hiresOperations)
                            return std::format("SCD 0x{:X}", nibbles[3]);
                        break;
                }
                break;
            case 0x1: return std::format("JP 0x{:03X}", addr);
            case 0x2: return std::format("CALL 0x{:03X}", addr);
            case 0x3: return std::format("SE V{:X}, 0x{:02X}", x, byte);
            case 0x4: return std::format("SNE V{:X}, 0x{:02X}", x, byte);
            case 0x5: return std::format("SE V{:X}, V{:X}", x, y);
            case 0x6: return std::format("LD V{:X}, 0x{:02X}", x, byte);
            case 0x7: return std::format("ADD V{:X}, 0x{:02X}", x, byte);
            case 0x8: 
                switch (nibbles[3])
                {
                    case 0x0: return std::format("LD V{:X}, V{:X}", x, y);
                    case 0x1: return std::format("OR V{:X}, V{:X}", x, y);
                    case 0x2: return std::format("AND V{:X}, V{:X}", x, y);
                    case 0x3: return std::format("XOR V{:X}, V{:X}", x, y);
                    case 0x4: return std::format("ADD V{:X}, V{:X}", x, y);
                    case 0x5: return std::format("SUB V{:X}, V{:X}", x, y);
                    case 0x6: return config.vyShifting ? std::format("SHR V{:X}, V{:X}", x, y) : std::format("SHR V{:X}", x);
                    case 0x7: return std::format("SUBN V{:X}, V{:X}", x, y);
                    case 0xE: return config.vyShifting ? std::format("SHL V{:X}, V{:X}", x, y) : std::format("SHL V{:X}", x);
                }
                break;
            case 0x9: return std::format("SNE V{:X}, V{:X}", x, y);
            case 0xA: return std::format("LD I, 0x{:03X}", addr);
            case 0xB: return config.vxJumping ? std::format("JP V{:X}, 0x{:02X}", x, byte) : std::format("JP V0, 0x{:03X}", addr);
            case 0xC: return std::format("RND V{:X}, 0x{:02X}", x, byte);
            case 0xD: 
                switch (nibbles[3])
                {
                    case 0x0: 
                        if (config.hiresOperations) 
                            return std::format("DRWH V{:X}, V{:X}", x, y);
                        [[fallthrough]];
                    default: return std::format("DRW V{:X}, V{:X}, 0x{:X}", x, y, nibbles[3]);
                }
                break;
            case 0xE:
                switch (byte)
                {
                    case 0x9E: return std::format("SKP V{:X}", x);
                    case 0xA1: return std::format("SKNP V{:X}", x);
                }
                break;
            case 0xF:
                switch (byte)
                {
                    case 0x07: return std::format("RDDT V{:X}", x);
                    case 0x0A: return std::format("LDK V{:X}", x);
                    case 0x15: return std::format("WRDT V{:X}", x);
                    case 0x18: return std::format("WRST V{:X}", x);
                    case 0x1E: return std::format("ADDI V{:X}", x);
                    case 0x29: return std::format("LDF V{:X}", x);
                    case 0x30: 
                        if (config.hiresOperations) 
                            return std::format("LDFH V{:X}", x);
                        break;
                    case 0x33: return std::format("LDB V{:X}", x);
                    case 0x55: return std::format("WRI V{:X}", x);
                    case 0x65: return std::format("RDI V{:X}", x);
                    case 0x75: return std::format("WRRPL V{:X}", x);
                    case 0x85: return std::format("RDRPL V{:X}", x);
                }
                break;
        }

        return std::format("0x{:04X}", instruction);
    }

    std::vector<std::string> Disassembler::disassemble(std::span<const std::uint16_t> memory)
    {
        std::vector<std::string> output(memory.size());

        for (size_t i = 0; i < memory.size(); ++i)
            output[i] = disassemble(memory[i]);

        return output;
    }
}