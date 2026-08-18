#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <span>

namespace disassembler::core
{
    struct DisassemblerConfig
    {
        std::uint16_t programStart = 0x200;
        bool vyShifting = false;
        bool vxJumping = false;
        bool hiresOperations = false;
    };

    class Disassembler
    {
    public:
        Disassembler(DisassemblerConfig config);

        std::uint16_t getProgramStart() const;

        std::string disassemble(std::uint16_t instruction);
        std::vector<std::string> disassemble(std::span<const std::uint16_t> memory);

    private:
        DisassemblerConfig config;
    };
}