#include "memory.hpp"

Memory::Memory(const MemoryLayout& memoryLayout) : layout(memoryLayout)
{
    if (layout.reserved_end >= layout.program_end || layout.reserved_data.size() > layout.reserved_end)
        throw InvalidMemoryLayoutException(layout.reserved_end, layout.program_end);

    memory.resize(layout.program_end);

    for (size_t i = 0; i < layout.reserved_data.size(); ++i)
        memory[i] = layout.reserved_data[i];
}

std::uint8_t Memory::read(std::uint16_t address) const
{
    if (address >= layout.program_end)
        return 0xFF;

    return memory[address];
}

std::vector<std::uint8_t> Memory::read_bytes(const std::uint16_t address, const std::uint16_t bytes) const
{
    std::vector<std::uint8_t> output(bytes);

    for (auto i = 0; i < bytes; ++i)
        output[i] = read(address + i);

    return output;
}

void Memory::write(std::uint16_t address, std::uint8_t data)
{
    if ((address < layout.reserved_end && layout.reserved_read_only) || address >= layout.program_end)
        return;

    memory[address] = data;
}