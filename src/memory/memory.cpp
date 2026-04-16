#include "memory.hpp"

Memory::Memory(const MemoryLayout& memoryLayout) : layout(memoryLayout)
{
    memory.resize(layout.program_end + 1);
}

std::uint8_t Memory::read(std::uint16_t address) const
{
    return memory[address];
}

std::vector<std::uint8_t> Memory::read_bytes(const std::uint16_t address, const std::uint16_t bytes) const
{
    std::vector<std::uint8_t> output(bytes);

    for (auto i = 0; i < bytes; ++i)
    {
        output[i] = memory[address + i];
    }

    return output;
}

void Memory::write(std::uint16_t address, std::uint8_t data)
{
    if (address < layout.reserved_end && layout.reserved_read_only)
        throw ReadOnlyMemoryException(address);

    memory[address] = data;
}