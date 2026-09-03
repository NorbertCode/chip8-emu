#include "memory.hpp"
#include <algorithm>

namespace chip8::core
{
    Memory::Memory(MemoryConfig memoryConfig, std::span<const std::uint8_t> reservedData) 
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

    std::vector<std::uint8_t> Memory::readBytes(std::uint16_t address, std::uint16_t bytes) const
    {
        std::vector<std::uint8_t> output(bytes);

        for (size_t i = 0; i < bytes; ++i)
            output[i] = read(address + static_cast<std::uint16_t>(i));

        return output;
    }

    void Memory::write(std::uint16_t address, std::uint8_t data)
    {
        if ((address < memoryConfig.reservedEnd && memoryConfig.reservedReadOnly) || address >= memoryConfig.programEnd)
            return;

        forceWrite(address, data);
    }

    void Memory::writeBytes(std::uint16_t address, const std::span<const std::uint8_t> data)
    {
        for (size_t i = 0; i < data.size(); ++i)
            write(address + static_cast<std::uint16_t>(i), data[i]);
    }

    void Memory::forceWrite(std::uint16_t address, std::uint8_t data)
    {
        memory[address] = data;
    }

    void Memory::forceWriteBytes(std::uint16_t address, const std::span<const std::uint8_t> data)
    {
        for (size_t i = 0; i < data.size(); ++i)
            forceWrite(address + static_cast<std::uint16_t>(i), data[i]);
    }

    void Memory::clear()
    {
        std::ranges::fill(memory, std::uint8_t{0});
    }

    const MemoryConfig& Memory::getMemoryConfig() const
    {
        return memoryConfig;
    }
}