#include <gtest/gtest.h>
#include "disassembler.hpp"

using namespace disassembler::core;

const DisassemblerConfig defaultConfig = {
    .vyShifting = false,
    .vxJumping = false,
    .hiresOperations = false
};

TEST(DisassemblerTest, Disassemble_CLS)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00E0;
    std::string expected = "CLS";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_RET)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00EE;
    std::string expected = "RET";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_SCR)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00FB;
    std::string expected = "0x00FB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_SCR)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x00FB;
    std::string expected = "SCR";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_SCL)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00FC;
    std::string expected = "0x00FC";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_SCL)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x00FC;
    std::string expected = "SCL";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_EXIT)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00FD;
    std::string expected = "EXIT";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_LOW)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00FE;
    std::string expected = "0x00FE";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_LOW)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x00FE;
    std::string expected = "LOW";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_HIGH)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00FF;
    std::string expected = "0x00FF";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_HIGH)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x00FF;
    std::string expected = "HIGH";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_SCD)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x00CA;
    std::string expected = "0x00CA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_SCD)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x00CA;
    std::string expected = "SCD 0xA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_JP)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x123A;
    std::string expected = "JP 0x23A";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_CALL)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x234A;
    std::string expected = "CALL 0x34A";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SE_Vx_Byte)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x3A4B;
    std::string expected = "SE VA, 0x4B";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SNE_Vx_Byte)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x4A5B;
    std::string expected = "SNE VA, 0x5B";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SE_Vx_Vy)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x5A60;
    std::string expected = "SE VA, V6";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_LD_Vx_Byte)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x6A7B;
    std::string expected = "LD VA, 0x7B";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_ADD_Vx_Byte)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x7A8B;
    std::string expected = "ADD VA, 0x8B";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_LD_Vx_Vy)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB0;
    std::string expected = "LD VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_OR)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB1;
    std::string expected = "OR VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_AND)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB2;
    std::string expected = "AND VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_XOR)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB3;
    std::string expected = "XOR VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_ADD_Vx_Vy)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB4;
    std::string expected = "ADD VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SUB)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB5;
    std::string expected = "SUB VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_vyShiftingDisabled_SHR)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB6;
    std::string expected = "SHR VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_vyShiftingEnabled_SHR)
{
    DisassemblerConfig config = defaultConfig;
    config.vyShifting = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x8AB6;
    std::string expected = "SHR VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SUBN)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8AB7;
    std::string expected = "SUBN VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_vyShiftingDisabled_SHL)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x8ABE;
    std::string expected = "SHL VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_vyShiftingEnabled_SHL)
{
    DisassemblerConfig config = defaultConfig;
    config.vyShifting = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0x8ABE;
    std::string expected = "SHL VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SNE_Vx_Vy)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0x9AB0;
    std::string expected = "SNE VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_LD_I_Addr)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xA123;
    std::string expected = "LD I, 0x123";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_vxJumpingDisabled_JP_V0_Addr)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xBCDE;
    std::string expected = "JP V0, 0xCDE";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_vxJumpingEnabled_JP_Vx_Addr)
{
    DisassemblerConfig config = defaultConfig;
    config.vxJumping = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0xBCDE;
    std::string expected = "JP VC, 0xDE";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_RND)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xCA1B;
    std::string expected = "RND VA, 0x1B";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_DRWH)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xDAB0;
    std::string expected = "DRW VA, VB, 0x0";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_DRWH)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0xDAB0;
    std::string expected = "DRWH VA, VB";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_DRW)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xDABC;
    std::string expected = "DRW VA, VB, 0xC";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SKP)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xEA9E;
    std::string expected = "SKP VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_SKNP)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xEAA1;
    std::string expected = "SKNP VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_RDDT)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA07;
    std::string expected = "RDDT VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_LDK)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA0A;
    std::string expected = "LDK VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_WRDT)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA15;
    std::string expected = "WRDT VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_WRST)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA18;
    std::string expected = "WRST VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_ADDI)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA1E;
    std::string expected = "ADDI VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_LDF)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA29;
    std::string expected = "LDF VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingDisabled_LDFH)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA30;
    std::string expected = "0xFA30";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_HiresDrawingEnabled_LDFH)
{
    DisassemblerConfig config = defaultConfig;
    config.hiresOperations = true;
    Disassembler disassembler(config);
    std::uint16_t input = 0xFA30;
    std::string expected = "LDFH VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_LDB)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA33;
    std::string expected = "LDB VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_WRI)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA55;
    std::string expected = "WRI VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_RDI)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA65;
    std::string expected = "RDI VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_WRRPL)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA75;
    std::string expected = "WRRPL VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_RDRPL)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFA85;
    std::string expected = "RDRPL VA";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_UnknownInstruction)
{
    Disassembler disassembler(defaultConfig);
    std::uint16_t input = 0xFFFF;
    std::string expected = "0xFFFF";

    std::string output = disassembler.disassemble(input);

    EXPECT_EQ(output, expected);
}

TEST(DisassemblerTest, Disassemble_Memory)
{
    Disassembler disassembler(defaultConfig);
    std::vector<std::uint8_t> memory = { 
        0x00, 0xE0, 
        0x12, 0x34,
        0x6A, 0x12
    };
    std::vector<std::string> expected = { 
        "CLS", 
        "JP 0x234", 
        "LD VA, 0x12"
    };

    std::vector<std::string> output = disassembler.disassemble(memory);

    EXPECT_EQ(output, expected);
}