#include <gtest/gtest.h>
#include "memory/memory.hpp"

const MemoryLayout layout = {
    .reserved_data = { 0x01, 0x02, 0x03 },
    .reserved_end = 0xF,
    .program_end = 0xFF,
    .reserved_read_only = true
};

TEST(MemoryTest, Constructor_InvalidMemoryLayout_ThrowsInvalidMemoryLayout)
{
    const MemoryLayout invalidLayout = {
        .reserved_end = 0x10,
        .program_end = 0xF,
        .reserved_read_only = true
    };

    EXPECT_THROW((Memory(invalidLayout)), InvalidMemoryLayoutException);
}

TEST(MemoryTest, Constructor_ReservedData_GetsCopiedCorrectly)
{
    Memory memory(layout);

    EXPECT_EQ(memory.read(0), 0x01);
    EXPECT_EQ(memory.read(1), 0x02);
    EXPECT_EQ(memory.read(2), 0x03);
    EXPECT_EQ(memory.read(3), 0x00);
}

TEST(MemoryTest, ReadWrite_WithinBounds_WritesAndReadsCorrectly)
{
    Memory memory(layout);

    memory.write(0x10, 0xAB);

    EXPECT_EQ(memory.read(0x10), 0xAB);
}

TEST(MemoryTest, ReadWrite_AtBorder_WritesAndReadsCorrectly)
{
    Memory memory(layout);

    memory.write(0xF, 0xAB);
    memory.write(0xFE, 0xCD);

    EXPECT_EQ(memory.read(0xF), 0xAB);
    EXPECT_EQ(memory.read(0xFE), 0xCD);
}

TEST(MemoryTest, Read_AboveProgramEnd_ReturnsFF)
{
    Memory memory(layout);

    EXPECT_EQ(memory.read(0xFF), 0xFF);
    EXPECT_EQ(memory.read(0x100), 0xFF);
}

TEST(MemoryTest, Write_AboveProgramEnd_DoesNothing)
{
    Memory memory(layout);

    memory.write(0xFF, 0xAB);
    memory.write(0x100, 0xCD);

    EXPECT_EQ(memory.read(0xFF), 0xFF);
    EXPECT_EQ(memory.read(0x100), 0xFF);
}

TEST(MemoryTest, Write_ReservedReadOnly_DoesNothing)
{
    Memory memory(layout);

    memory.write(0xD, 0xAB);
    memory.write(0xE, 0xCD);

    EXPECT_EQ(memory.read(0xD), 0x0);
    EXPECT_EQ(memory.read(0xE), 0x0);
}

TEST(MemoryTest, ReadBytes_WithinBounds_ReadsCorrectly)
{
    Memory memory(layout);
    std::vector<std::uint8_t> expected = { 0xAB, 0xCD, 0xEF };

    memory.write(0x10, 0xAB);
    memory.write(0x11, 0xCD);
    memory.write(0x12, 0xEF);

    EXPECT_EQ(memory.read_bytes(0x10, 3), expected);
}

TEST(MemoryTest, ReadBytes_AboveProgramEnd_ReturnsFF)
{
    Memory memory(layout);
    std::vector<std::uint8_t> expected = { 0xFF, 0xFF, 0xFF };

    const std::vector<std::uint8_t> output = memory.read_bytes(0xFF, 3);

    EXPECT_EQ(output, expected);
}

TEST(MemoryTest, ReadBytes_OverlappingProgramEnd_ReturnsFF)
{
    Memory memory(layout);
    std::vector<std::uint8_t> expected = { 0xAB, 0xCD, 0xFF };

    memory.write(0xFD, 0xAB);
    memory.write(0xFE, 0xCD);
    memory.write(0xFF, 0xEF);

    EXPECT_EQ(memory.read_bytes(0xFD, 3), expected);
}