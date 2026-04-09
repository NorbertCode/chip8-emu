#include "memory.hpp"

Memory::Memory(const MemoryLayout& memoryLayout) : layout(memoryLayout)
{
    memory.resize(layout.program_end + 1);
}

std::uint8_t Memory::read(std::uint16_t address) const
{
    return memory[address];
}

void Memory::write(std::uint16_t address, std::uint8_t data)
{
    if (address < layout.reserved_end && layout.reserved_read_only)
        throw ReadOnlyMemoryException(address);

    memory[address] = data;
}