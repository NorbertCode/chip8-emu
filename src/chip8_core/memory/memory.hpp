#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>

struct MemoryLayout
{
    std::vector<uint8_t> reserved_data;
    std::uint16_t reserved_end;
    std::uint16_t program_end; // This also works as memory end
    bool reserved_read_only;
};

class Memory
{
public:
    Memory(const MemoryLayout& memoryLayout);

    std::uint8_t read(std::uint16_t address) const;
    std::vector<std::uint8_t> read_bytes(std::uint16_t address, std::uint16_t bytes) const;

    void write(std::uint16_t address, std::uint8_t data);
    void write_bytes(std::uint16_t address, const std::vector<std::uint8_t>& data);

private:
    std::vector<std::uint8_t> memory;

    const MemoryLayout& layout;
};

class InvalidMemoryLayoutException : public std::logic_error
{
public:
    InvalidMemoryLayoutException(const std::uint16_t reserved_end, const std::uint16_t program_end) 
        : std::logic_error("Invalid memory layout. Reservered end (" + std::to_string(reserved_end) + ") has to be < program end (" + std::to_string(program_end) + ") and data size has to be < reserved end"), reserved_end(reserved_end), program_end(program_end) { }

    const std::uint16_t getReservedEnd() const { return reserved_end; }
    const std::uint16_t getProgramEnd() const { return program_end; }

private:
    const std::uint16_t reserved_end;
    const std::uint16_t program_end;
};