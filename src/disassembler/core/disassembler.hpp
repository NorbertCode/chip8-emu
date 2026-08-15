#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct DisassemblerConfig
{
    bool vyShifting = false;
    bool vxJumping = false;
    bool hiresOperations = false;
};

class Disassembler
{
public:
    Disassembler(DisassemblerConfig config);

    std::string disassemble(std::uint16_t instruction);
    std::vector<std::string> disassemble(const std::vector<std::uint8_t>& memory);

private:
    DisassemblerConfig config;
};