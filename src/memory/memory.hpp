#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>

struct MemoryLayout
{
    std::uint16_t reserved_end;
    std::uint16_t program_start;
    std::uint16_t program_end; // This also works as memory end
    bool reserved_read_only;
};

class Memory
{
public:
    Memory(const MemoryLayout& memoryLayout);

    std::uint8_t read(const std::uint16_t address) const;
    std::vector<std::uint8_t> read_bytes(const std::uint16_t address, const std::uint16_t bytes) const;
    void write(const std::uint16_t address, const std::uint8_t data);

private:
    std::vector<std::uint8_t> memory;
    const MemoryLayout layout;
};

class ReadOnlyMemoryException : public std::logic_error
{
public:
    ReadOnlyMemoryException(const std::uint16_t address)
        : std::logic_error("Memory at address " + std::to_string(address)), address(address) { }

    const std::uint16_t getAddress() const { return address; }

private:
    const std::uint16_t address;
};