#include "memory.hpp"

Memory::Memory(const MemoryConfig& memoryConfig, const std::vector<std::uint8_t>& reservedData) 
    : memoryConfig(memoryConfig)
{
    if (memoryConfig.reservedEnd >= memoryConfig.programEnd || reservedData.size() > memoryConfig.reservedEnd)
        throw InvalidMemoryLayoutException(memoryConfig.reservedEnd, memoryConfig.programEnd);

    memory.resize(memoryConfig.programEnd);

    for (size_t i = 0; i < reservedData.size(); ++i)
        memory[i] = reservedData[i];
}

std::uint8_t Memory::read(std::uint16_t address) const
{
    if (address >= memoryConfig.programEnd)
        return 0xFF;

    return memory[address];
}

std::vector<std::uint8_t> Memory::read_bytes(std::uint16_t address, std::uint16_t bytes) const
{
    std::vector<std::uint8_t> output(bytes);

    for (size_t i = 0; i < bytes; ++i)
        output[i] = read(address + i);

    return output;
}

void Memory::write(std::uint16_t address, std::uint8_t data)
{
    if ((address < memoryConfig.reservedEnd && memoryConfig.reservedReadOnly) || address >= memoryConfig.programEnd)
        return;

    memory[address] = data;
}

void Memory::write_bytes(std::uint16_t address, const std::vector<std::uint8_t>& data)
{
    for (size_t i = 0; i < data.size(); ++i)
        write(address + i, data[i]);
}

const MemoryConfig& Memory::getMemoryConfig() const
{
    return memoryConfig;
}