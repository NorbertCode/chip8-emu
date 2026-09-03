#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <span>
#include <stdexcept>

namespace chip8::core
{
    struct MemoryConfig
    {
        std::uint16_t reservedEnd;
        std::uint16_t programEnd; // This also works as memory end
        bool reservedReadOnly;
    };

    class Memory
    {
    public:
        Memory(MemoryConfig memoryConfig, std::span<const std::uint8_t> reservedData = {});

        std::uint8_t read(std::uint16_t address) const;
        std::vector<std::uint8_t> readBytes(std::uint16_t address, std::uint16_t bytes) const;

        void write(std::uint16_t address, std::uint8_t data);
        void writeBytes(std::uint16_t address, std::span<const std::uint8_t> data);

        void forceWrite(std::uint16_t address, std::uint8_t data);
        void forceWriteBytes(std::uint16_t address, std::span<const std::uint8_t> data);

        void clear();

        const MemoryConfig& getMemoryConfig() const;

    private:
        std::vector<std::uint8_t> memory;

        MemoryConfig memoryConfig;
    };

    class InvalidMemoryLayoutException : public std::logic_error
    {
    public:
        InvalidMemoryLayoutException(const std::uint16_t reservedEnd, const std::uint16_t programEnd) 
            : std::logic_error("Invalid memory layout. Reservered end (" + std::to_string(reservedEnd) + ") has to be < program end (" + std::to_string(programEnd) + ") and data size has to be < reserved end"), reservedEnd(reservedEnd), programEnd(programEnd) { }

        std::uint16_t getReservedEnd() const { return reservedEnd; }
        std::uint16_t getProgramEnd() const { return programEnd; }

    private:
        std::uint16_t reservedEnd;
        std::uint16_t programEnd;
    };
}