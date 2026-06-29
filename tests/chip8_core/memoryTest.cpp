#include <gtest/gtest.h>
#include "memory/memory.hpp"

const MemoryConfig memoryConfig = {
    .reservedEnd = 0xF,
    .programEnd = 0xFF,
    .reservedReadOnly = true
};

const std::vector<std::uint8_t> reservedData = {
    0x01, 0x02, 0x03
};

TEST(MemoryTest, Constructor_InvalidMemoryLayout_ThrowsInvalidMemoryLayout)
{
    const MemoryConfig invalidLayout = {
        .reservedEnd = 0x10,
        .programEnd = 0xF,
        .reservedReadOnly = true
    };

    EXPECT_THROW((Memory(invalidLayout)), InvalidMemoryLayoutException);
}

TEST(MemoryTest, Constructor_ReservedData_GetsCopiedCorrectly)
{
    Memory memory(memoryConfig, reservedData);

    EXPECT_EQ(memory.read(0), 0x01);
    EXPECT_EQ(memory.read(1), 0x02);
    EXPECT_EQ(memory.read(2), 0x03);
    EXPECT_EQ(memory.read(3), 0x00);
}

TEST(MemoryTest, ReadWrite_WithinBounds_WritesAndReadsCorrectly)
{
    Memory memory(memoryConfig);

    memory.write(0x10, 0xAB);

    EXPECT_EQ(memory.read(0x10), 0xAB);
}

TEST(MemoryTest, ReadWrite_AtBorder_WritesAndReadsCorrectly)
{
    Memory memory(memoryConfig);

    memory.write(0xF, 0xAB);
    memory.write(0xFE, 0xCD);

    EXPECT_EQ(memory.read(0xF), 0xAB);
    EXPECT_EQ(memory.read(0xFE), 0xCD);
}

TEST(MemoryTest, Read_AboveProgramEnd_ReturnsFF)
{
    Memory memory(memoryConfig);

    EXPECT_EQ(memory.read(0xFF), 0xFF);
    EXPECT_EQ(memory.read(0x100), 0xFF);
}

TEST(MemoryTest, Write_AboveProgramEnd_DoesNothing)
{
    Memory memory(memoryConfig);

    memory.write(0xFF, 0xAB);
    memory.write(0x100, 0xCD);

    EXPECT_EQ(memory.read(0xFF), 0xFF);
    EXPECT_EQ(memory.read(0x100), 0xFF);
}

TEST(MemoryTest, Write_ReservedReadOnly_DoesNothing)
{
    Memory memory(memoryConfig);

    memory.write(0xD, 0xAB);
    memory.write(0xE, 0xCD);

    EXPECT_EQ(memory.read(0xD), 0x0);
    EXPECT_EQ(memory.read(0xE), 0x0);
}

TEST(MemoryTest, ReadBytes_WithinBounds_ReadsCorrectly)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> expected = { 0xAB, 0xCD, 0xEF };

    memory.write(0x10, 0xAB);
    memory.write(0x11, 0xCD);
    memory.write(0x12, 0xEF);

    EXPECT_EQ(memory.read_bytes(0x10, 3), expected);
}

TEST(MemoryTest, ReadBytes_AboveProgramEnd_ReturnsFF)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> expected = { 0xFF, 0xFF, 0xFF };

    const std::vector<std::uint8_t> output = memory.read_bytes(0xFF, 3);

    EXPECT_EQ(output, expected);
}

TEST(MemoryTest, ReadBytes_OverlappingProgramEnd_ReturnsFF)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> expected = { 0xAB, 0xCD, 0xFF };

    memory.write(0xFD, 0xAB);
    memory.write(0xFE, 0xCD);
    memory.write(0xFF, 0xEF);

    EXPECT_EQ(memory.read_bytes(0xFD, 3), expected);
}

TEST(MemoryTest, WriteBytes_WithinBounds_WritesCorrectly)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> data = { 0xAB, 0xCD, 0xEF };

    memory.write_bytes(0x10, data);

    EXPECT_EQ(memory.read(0x10), 0xAB);
    EXPECT_EQ(memory.read(0x11), 0xCD);
    EXPECT_EQ(memory.read(0x12), 0xEF);
}

TEST(MemoryTest, WriteBytes_AboveProgramEnd_DoesNothing)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> data = { 0xAB, 0xCD, 0xEF };

    memory.write_bytes(0xFF, data);

    EXPECT_EQ(memory.read(0xFF), 0xFF);
}

TEST(MemoryTest, WriteBytes_OverlappingProgramEnd_WritesPartially)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> data = { 0xAB, 0xCD, 0xEF };

    memory.write_bytes(0xFE, data);

    EXPECT_EQ(memory.read(0xFE), 0xAB);
    EXPECT_EQ(memory.read(0xFF), 0xFF);
    EXPECT_EQ(memory.read(0x100), 0xFF);
}

TEST(MemoryTest, WriteBytes_ReservedReadOnly_DoesNothing)
{
    Memory memory(memoryConfig);
    std::vector<std::uint8_t> data = { 0xAB, 0xCD, 0xEF };

    memory.write_bytes(0xC, data);

    EXPECT_EQ(memory.read(0xC), 0x0);
    EXPECT_EQ(memory.read(0xD), 0x0);
    EXPECT_EQ(memory.read(0xE), 0x0);
}