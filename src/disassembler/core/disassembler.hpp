#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Disassembler
{
public:
    static std::string disassemble(std::uint16_t instruction);
    static std::vector<std::string> disassemble(const std::vector<std::uint8_t>& memory);
};