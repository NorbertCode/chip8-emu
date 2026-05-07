#include <gtest/gtest.h>
#include "memory/memory.hpp"
#include "processor/processor.hpp"
#include "peripherals/display.hpp"

class ProcessorTest : public ::testing::Test
{
protected:
    Processor processor;

    const MemoryLayout layout = {
        .reserved_data = { 
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0 character
            0x20, 0x60, 0x20, 0x20, 0x70 // 1 character
        },
        .reserved_end = 0x200,
        .program_end = 0x1000,
        .reserved_read_only = true
    };

    const Quirks quirks = {
        .vfReset = false,
        .indexIncrement = false,
        .displayClipping = false,
        .vyShifting = false,
        .vxJumping = false
    };

    Display display;
    Memory memory;
    Keyboard keyboard;

    ProcessorTest() 
        : memory(layout), display(64, 32), keyboard(keyboard), processor(memory, display, keyboard, quirks, 0x200) { }

};

TEST_F(ProcessorTest, Step_NotHalted_FetchesAndExecutesInstruction)
{
    memory.write(0x200, 0xAB);
    memory.write(0x201, 0xCD);

    processor.step();

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, Step_Halted_DoesNothing)
{
    processor.execute(0xF00A); // LD V0, K (Halts until key press)

    processor.step();

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
    EXPECT_EQ(processor.isHalted(), true);
}

TEST_F(ProcessorTest, TickTimers_AboveZero_TicksDown)
{
    processor.execute(0x6005); // LD V0, 0x05
    processor.execute(0x6106); // LD V1, 0x06
    processor.execute(0xF015); // LD DT, V0
    processor.execute(0xF118); // LD ST, V1

    processor.tickTimers();

    EXPECT_EQ(processor.getDelayTimer(), 0x04);
    EXPECT_EQ(processor.getSoundTimer(), 0x05);
}

TEST_F(ProcessorTest, TickTimers_AtZero_DoesNothing)
{
    processor.execute(0x6000); // LD V0, 0x00
    processor.execute(0xF015); // LD DT, V0
    processor.execute(0xF018); // LD ST, V0

    processor.tickTimers();

    EXPECT_EQ(processor.getDelayTimer(), 0x00);
    EXPECT_EQ(processor.getSoundTimer(), 0x00);
}

TEST_F(ProcessorTest, Fetch_IncrementsProgramCounter)
{
    memory.write(0x200, 0xAB);
    memory.write(0x201, 0xCD);
    memory.write(0x202, 0x12);
    memory.write(0x203, 0x34);

    const std::uint16_t instruction = processor.fetch();

    EXPECT_EQ(instruction, 0xABCD);
    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, cls_DisplayWithPixels_ClearsDisplay)
{
    display.xorPixel(0, 0, true, false);
    display.xorPixel(1, 1, true, false);

    processor.execute(0x00E0); // CLS

    for (size_t x = 0; x < display.getWidth(); ++x)
        for (size_t y = 0; y < display.getHeight(); ++y)
            EXPECT_FALSE(display.getPixel(x, y));
}

TEST_F(ProcessorTest, cls_CleanDisplay_DoesNothing)
{
    processor.execute(0x00E0); // CLS

    for (size_t x = 0; x < display.getWidth(); ++x)
        for (size_t y = 0; y < display.getHeight(); ++y)
            EXPECT_FALSE(display.getPixel(x, y));
}

TEST_F(ProcessorTest, ret_AddressOnStack_ReturnsFromSubroutine)
{
    processor.execute(0x2400); // CALL 0x400

    processor.execute(0x00EE); // RET

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, ret_EmptyStack_Underflows)
{
    processor.execute(0x2400); // CALL 0x400
    processor.execute(0x00EE); // RET

    processor.execute(0x00EE); // RET

    EXPECT_EQ(processor.getProgramCounter(), processor.getStack()[0xFF]);
    EXPECT_EQ(processor.getStackPointer(), 0xFF);
}

TEST_F(ProcessorTest, jp_JumpsToAddress)
{
    processor.execute(0x1400); // JP 0x400

    EXPECT_EQ(processor.getProgramCounter(), 0x400);
}

TEST_F(ProcessorTest, call_CallsSubroutine)
{
    processor.execute(0x2400); // CALL 0x400

    EXPECT_EQ(processor.getProgramCounter(), 0x400);
    EXPECT_EQ(processor.getStackPointer(), 1);
    EXPECT_EQ(processor.getStack()[0], 0x200);
}

TEST_F(ProcessorTest, seRegByte_Equal_SkipsNextInstruction)
{
    processor.execute(0x6012); // LD V0, 0x12

    processor.execute(0x3012); // SE V0, 0x12

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, seRegByte_NotEqual_DoesNotSkipNextInstruction)
{
    processor.execute(0x6012); // LD V0, 0x12

    processor.execute(0x3013); // SE V0, 0x13

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, sneRegByte_NotEqual_SkipsNextInstruction)
{
    processor.execute(0x6012); // LD V0, 0x12

    processor.execute(0x4013); // SNE V0, 0x13

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, sneRegByte_Equal_DoesNotSkipNextInstruction)
{
    processor.execute(0x6012); // LD V0, 0x12

    processor.execute(0x4012); // SNE V0, 0x12

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, seReg_Equal_SkipsNextInstruction)
{
    processor.execute(0x6012); // LD V0, 0x12
    processor.execute(0x6112); // LD V1, 0x12

    processor.execute(0x5010); // SE V0, V1

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, seReg_NotEqual_DoesNotSkipNextInstruction)
{
    processor.execute(0x6012); // LD V0, 0x12
    processor.execute(0x6113); // LD V1, 0x13

    processor.execute(0x5010); // SE V0, V1

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, ldRegByte_LoadsByteIntoRegister)
{
    processor.execute(0x60AB); // LD V0, 0xAB

    EXPECT_EQ(processor.getRegistersV()[0], 0xAB);
}

TEST_F(ProcessorTest, addRegByte_AddsByteToRegister)
{
    processor.execute(0x6001); // LD V0, 0x01

    processor.execute(0x7002); // ADD V0, 0x02

    EXPECT_EQ(processor.getRegistersV()[0], 0x03);
}

TEST_F(ProcessorTest, ldReg_LoadsRegisterIntoRegister)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x8010); // LD V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x02);
}

TEST_F(ProcessorTest, orReg_OrsRegisters)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x8011); // OR V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x03);
}

TEST_F(ProcessorTest, orReg_vfResetDisabled_DoesNotResetVf)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02
    processor.execute(0x6FFF); // LD VF, 0xFF

    processor.execute(0x8011); // OR V0, V1

    EXPECT_EQ(processor.getRegistersV()[0xF], 0xFF);
}

TEST_F(ProcessorTest, orReg_vfResetEnabled_ResetsVf)
{
    Processor processorWithVfReset(memory, display, keyboard, Quirks{ .vfReset = true }, 0x200);

    processorWithVfReset.execute(0x6001); // LD V0, 0x01
    processorWithVfReset.execute(0x6102); // LD V1, 0x02
    processorWithVfReset.execute(0x6FFF); // LD VF, 0xFF

    processorWithVfReset.execute(0x8011); // OR V0, V1

    EXPECT_EQ(processorWithVfReset.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, andReg_AndsRegisters)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6103); // LD V1, 0x03

    processor.execute(0x8012); // AND V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x01);
}

TEST_F(ProcessorTest, andReg_vfResetDisabled_DoesNotResetVf)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6103); // LD V1, 0x03
    processor.execute(0x6FFF); // LD VF, 0xFF

    processor.execute(0x8012); // AND V0, V1

    EXPECT_EQ(processor.getRegistersV()[0xF], 0xFF);
}

TEST_F(ProcessorTest, andReg_vfResetEnabled_ResetsVf)
{
    Processor processorWithVfReset(memory, display, keyboard, Quirks{ .vfReset = true }, 0x200);
    processorWithVfReset.execute(0x6001); // LD V0, 0x01
    processorWithVfReset.execute(0x6103); // LD V1, 0x03
    processorWithVfReset.execute(0x6FFF); // LD VF, 0xFF

    processorWithVfReset.execute(0x8012); // AND V0, V1

    EXPECT_EQ(processorWithVfReset.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, xorReg_XorsRegisters)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6103); // LD V1, 0x03

    processor.execute(0x8013); // XOR V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x02);
}

TEST_F(ProcessorTest, xorReg_vfResetDisabled_DoesNotResetVf)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6103); // LD V1, 0x03
    processor.execute(0x6FFF); // LD VF, 0xFF

    processor.execute(0x8013); // XOR V0, V1

    EXPECT_EQ(processor.getRegistersV()[0xF], 0xFF);
}

TEST_F(ProcessorTest, xorReg_vfResetEnabled_ResetsVf)
{
    Processor processorWithVfReset(memory, display, keyboard, Quirks{ .vfReset = true }, 0x200);
    processorWithVfReset.execute(0x6001); // LD V0, 0x01
    processorWithVfReset.execute(0x6103); // LD V1, 0x03
    processorWithVfReset.execute(0x6FFF); // LD VF, 0xFF

    processorWithVfReset.execute(0x8013); // XOR V0, V1

    EXPECT_EQ(processorWithVfReset.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, addReg_WontOverflow_AddsRegistersNoCarry)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x8014); // ADD V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x03);
    EXPECT_EQ(processor.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, addReg_WillOverflow_AddsRegistersWithCarry)
{
    processor.execute(0x60FF); // LD V0, 0xFF
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x8014); // ADD V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x01);
    EXPECT_EQ(processor.getRegistersV()[0xF], 1);
}

TEST_F(ProcessorTest, subReg_WontUnderflow_SubsRegistersNoBorrow)
{
    processor.execute(0x6003); // LD V0, 0x03
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x8015); // SUB V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x01);
    EXPECT_EQ(processor.getRegistersV()[0xF], 1);
}

TEST_F(ProcessorTest, subReg_WillUnderflow_SubsRegistersWithBorrow)
{
    processor.execute(0x6002); // LD V0, 0x02
    processor.execute(0x6103); // LD V1, 0x03

    processor.execute(0x8015); // SUB V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0xFF);
    EXPECT_EQ(processor.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, shrReg_LsbZero_ShiftsRightWithZeroLsb)
{
    processor.execute(0x6002); // LD V0, 0x02
    processor.execute(0x6104); // LD V1, 0x04

    processor.execute(0x8016); // SHR V0 {, V1}

    EXPECT_EQ(processor.getRegistersV()[0], 0x01); // V1 should be ignored
    EXPECT_EQ(processor.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, shrReg_LsbOne_ShiftsRightWithOneLsb)
{
    processor.execute(0x6003); // LD V0, 0x03
    processor.execute(0x6104); // LD V1, 0x04

    processor.execute(0x8016); // SHR V0 {, V1}

    EXPECT_EQ(processor.getRegistersV()[0], 0x01);
    EXPECT_EQ(processor.getRegistersV()[0xF], 1);
}

TEST_F(ProcessorTest, shrReg_vyShiftingEnabled_ShiftsVyInsteadOfVx)
{
    Processor processorWithVyShifting(memory, display, keyboard, Quirks{ .vyShifting = true }, 0x200);
    processorWithVyShifting.execute(0x6002); // LD V0, 0x02
    processorWithVyShifting.execute(0x6104); // LD V1, 0x04

    processorWithVyShifting.execute(0x8016); // SHR V0 {, V1}

    EXPECT_EQ(processorWithVyShifting.getRegistersV()[0], 0x02);
}

TEST_F(ProcessorTest, subnReg_WontUnderflow_SubsRegistersNoBorrow)
{
    processor.execute(0x6002); // LD V0, 0x02
    processor.execute(0x6103); // LD V1, 0x03

    processor.execute(0x8017); // SUBN V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0x01);
    EXPECT_EQ(processor.getRegistersV()[0xF], 1);
}

TEST_F(ProcessorTest, subnReg_WillUnderflow_SubsRegistersWithBorrow)
{
    processor.execute(0x6003); // LD V0, 0x03
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x8017); // SUBN V0, V1

    EXPECT_EQ(processor.getRegistersV()[0], 0xFF);
    EXPECT_EQ(processor.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, shlReg_MsbZero_ShiftsLeftWithZeroMsb)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x801E); // SHL V0 {, V1}

    EXPECT_EQ(processor.getRegistersV()[0], 0x02); // V1 should be ignored
    EXPECT_EQ(processor.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, shlReg_MsbOne_ShiftsLeftWithOneMsb)
{
    processor.execute(0x60FF); // LD V0, 0xFF
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x801E); // SHL V0 {, V1}

    EXPECT_EQ(processor.getRegistersV()[0], 0xFE); // V1 should be ignored
    EXPECT_EQ(processor.getRegistersV()[0xF], 1);
}

TEST_F(ProcessorTest, shlReg_vyShiftingEnabled_ShiftsVyInsteadOfVx)
{
    Processor processorWithVyShifting(memory, display, keyboard, Quirks{ .vyShifting = true }, 0x200);
    processorWithVyShifting.execute(0x6001); // LD V0, 0x01
    processorWithVyShifting.execute(0x6102); // LD V1, 0x02

    processorWithVyShifting.execute(0x801E); // SHL V0 {, V1}

    EXPECT_EQ(processorWithVyShifting.getRegistersV()[0], 0x04);
}

TEST_F(ProcessorTest, sneReg_NotEqual_SkipsNextInstruction)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02

    processor.execute(0x9010); // SNE V0, V1

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, sneReg_Equal_DoesNotSkipNextInstruction)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6101); // LD V1, 0x01

    processor.execute(0x9010); // SNE V0, V1

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, ldI_LoadsAddressIntoRegisterI)
{
    processor.execute(0xA123); // LD I, 0x123

    EXPECT_EQ(processor.getRegisterI(), 0x123);
}

TEST_F(ProcessorTest, jpV0_vxJumpingDisabled_JumpsToAddressPlusV0)
{
    processor.execute(0x6001); // LD V0, 0x01

    processor.execute(0xB123); // JP V0, 0x123

    EXPECT_EQ(processor.getProgramCounter(), 0x124);
}

TEST_F(ProcessorTest, jpV0_vxJumpingEnabled_JumpsToAddressPlusVx)
{
    Processor processorWithVxJumping(memory, display, keyboard, Quirks{ .vxJumping = true }, 0x200);
    processorWithVxJumping.execute(0x6001); // LD V0, 0x01
    processorWithVxJumping.execute(0x6102); // LD V1, 0x02

    processorWithVxJumping.execute(0xB123); // JP V1, 0x123

    EXPECT_EQ(processorWithVxJumping.getProgramCounter(), 0x125);
}

TEST_F(ProcessorTest, rnd_GeneratesMaskedRandomNumbers)
{
    for (size_t i = 0; i < 100; ++i)
    {
        processor.execute(0xC0AB); // RND V0, 0xAB

        EXPECT_EQ(processor.getRegistersV()[0] & ~0xAB, 0x0);
    }
}

TEST_F(ProcessorTest, drw_NoCollision_DrawsSpriteNoCollisionFlag)
{
    memory.write(0x300, 0b11110000);
    memory.write(0x301, 0b10010000);
    memory.write(0x302, 0b11110000);
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xD013); // DRW V0, V1, 3

    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_TRUE(display.getPixel(1, 0));
    EXPECT_TRUE(display.getPixel(2, 0));
    EXPECT_TRUE(display.getPixel(3, 0));

    EXPECT_TRUE(display.getPixel(0, 1));
    EXPECT_FALSE(display.getPixel(1, 1));
    EXPECT_FALSE(display.getPixel(2, 1));
    EXPECT_TRUE(display.getPixel(3, 1));

    EXPECT_TRUE(display.getPixel(0, 2));
    EXPECT_TRUE(display.getPixel(1, 2));
    EXPECT_TRUE(display.getPixel(2, 2));
    EXPECT_TRUE(display.getPixel(3, 2));

    EXPECT_EQ(processor.getRegistersV()[0xF], 0);
}

TEST_F(ProcessorTest, drw_Collision_DrawsSpriteWithCollisionFlag)
{
    memory.write(0x300, 0b11110000);
    memory.write(0x301, 0b10010000);
    memory.write(0x302, 0b11110000);
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xD013); // DRW V0, V1, 3
    processor.execute(0xD013); // DRW V0, V1, 3

    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(display.getPixel(1, 0));
    EXPECT_FALSE(display.getPixel(2, 0));
    EXPECT_FALSE(display.getPixel(3, 0));

    EXPECT_FALSE(display.getPixel(0, 1));
    EXPECT_FALSE(display.getPixel(1, 1));
    EXPECT_FALSE(display.getPixel(2, 1));
    EXPECT_FALSE(display.getPixel(3, 1));

    EXPECT_FALSE(display.getPixel(0, 2));
    EXPECT_FALSE(display.getPixel(1, 2));
    EXPECT_FALSE(display.getPixel(2, 2));
    EXPECT_FALSE(display.getPixel(3, 2));

    EXPECT_EQ(processor.getRegistersV()[0xF], 1);
}

TEST_F(ProcessorTest, drw_DisplayClippingDisabled_WrapsAround)
{
    memory.write(0x300, 0b11111111);
    processor.execute(0xA300); // LD I, 0x300
    processor.execute(0x603E); // LD V0, 0x3E (62)
    processor.execute(0x6100); // LD V1, 0x00

    processor.execute(0xD011); // DRW V0, V1, 1

    EXPECT_TRUE(display.getPixel(62, 0));
    EXPECT_TRUE(display.getPixel(63, 0));
    EXPECT_TRUE(display.getPixel(0, 0));
    EXPECT_TRUE(display.getPixel(1, 0));
}

TEST_F(ProcessorTest, drw_DisplayClippingEnabled_DoesNotWrap)
{
    Processor processorWithClipping(memory, display, keyboard, Quirks{ .displayClipping = true }, 0x200);
    memory.write(0x300, 0b11111111);
    processorWithClipping.execute(0xA300); // LD I, 0x300
    processorWithClipping.execute(0x603E); // LD V0, 0x3E (62)
    processorWithClipping.execute(0x6100); // LD V1, 0x00

    processorWithClipping.execute(0xD011); // DRW V0, V1, 1

    EXPECT_TRUE(display.getPixel(62, 0));
    EXPECT_TRUE(display.getPixel(63, 0));
    EXPECT_FALSE(display.getPixel(0, 0));
    EXPECT_FALSE(display.getPixel(1, 0));
}

TEST_F(ProcessorTest, skp_KeyPressed_SkipsNextInstruction)
{
    processor.execute(0x6001); // LD V0, 01 - so SKP looks at key 1
    keyboard.keyDown(1);

    processor.execute(0xE09E); // SKP V0

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, skp_KeyNotPressed_DoesNotSkipNextInstruction)
{
    processor.execute(0x6001); // LD V0, 01 - so SKP looks at key 1
    keyboard.keyUp(1);

    processor.execute(0xE09E); // SKP V0

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, sknp_KeyNotPressed_SkipsNextInstruction)
{
    processor.execute(0x6001); // LD V0, 01 - so SKNP looks at key 1
    keyboard.keyUp(1);

    processor.execute(0xE0A1); // SKNP V0

    EXPECT_EQ(processor.getProgramCounter(), 0x202);
}

TEST_F(ProcessorTest, sknp_KeyPressed_DoesNotSkipNextInstruction)
{
    processor.execute(0x6001); // LD V0, 01 - so SKNP looks at key 1
    keyboard.keyDown(1);

    processor.execute(0xE0A1); // SKNP V0

    EXPECT_EQ(processor.getProgramCounter(), 0x200);
}

TEST_F(ProcessorTest, ldRegDT_LoadsDelayTimerIntoRegister)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0xF015); // LD DT, V0

    processor.execute(0xF107); // LD V1, DT

    EXPECT_EQ(processor.getDelayTimer(), 0x01);
    EXPECT_EQ(processor.getRegistersV()[1], 0x01);
}

TEST_F(ProcessorTest, ldK_HaltsUntilKeyPress)
{
    processor.execute(0xF00A); // LD V0, K

    EXPECT_TRUE(processor.isHalted());

    keyboard.keyUp(1);

    EXPECT_FALSE(processor.isHalted());
    EXPECT_EQ(processor.getRegistersV()[0], 1);
}

TEST_F(ProcessorTest, ldK_HaltsUntilKeyPressIgnoresOtherKeys)
{
    processor.execute(0xF00A); // LD V0, K

    EXPECT_TRUE(processor.isHalted());

    keyboard.keyUp(1);
    keyboard.keyUp(2);
    keyboard.keyUp(3);

    EXPECT_FALSE(processor.isHalted());
    EXPECT_EQ(processor.getRegistersV()[0], 1);
}

TEST_F(ProcessorTest, ldDTReg_LoadsRegisterIntoDelayTimer)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0xF015); // LD DT, V0

    EXPECT_EQ(processor.getDelayTimer(), 0x01);
}

TEST_F(ProcessorTest, ldSTReg_LoadsRegisterIntoSoundTimer)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0xF018); // LD ST, V0

    EXPECT_EQ(processor.getSoundTimer(), 0x01);
}

TEST_F(ProcessorTest, addI_LoadsSumIntoRegisterI)
{
    processor.execute(0xA100); // LD I, 0x100
    processor.execute(0x6001); // LD V0, 0x01

    processor.execute(0xF01E); // ADD I, V0

    EXPECT_EQ(processor.getRegisterI(), 0x101);
}

TEST_F(ProcessorTest, ldF_FirstCharacter_LoadsCorrectly)
{
    processor.execute(0x6000); // LD V0, 0x0

    processor.execute(0xF029); // LD F, Vx

    EXPECT_EQ(processor.getRegisterI(), 0x0);
}

TEST_F(ProcessorTest, ldF_SecondCharacter_LoadsCorrectly)
{
    processor.execute(0x6001); // LD V0, 0x1

    processor.execute(0xF029); // LD F, Vx

    EXPECT_EQ(processor.getRegisterI(), 0x5);
}

TEST_F(ProcessorTest, ldB_SingleDigit_ConvertsToBCD)
{
    processor.execute(0x6005); // LD V0, 0x05
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF033); // LD B, V0

    EXPECT_EQ(memory.read(0x300), 0);
    EXPECT_EQ(memory.read(0x301), 0);
    EXPECT_EQ(memory.read(0x302), 5);
}

TEST_F(ProcessorTest, ldB_TwoDigits_ConvertsToBCD)
{
    processor.execute(0x600A); // LD V0, 0x0A
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF033); // LD B, V0

    EXPECT_EQ(memory.read(0x300), 0);
    EXPECT_EQ(memory.read(0x301), 1);
    EXPECT_EQ(memory.read(0x302), 0);
}

TEST_F(ProcessorTest, ldB_ThreeDigits_ConvertsToBCD)
{
    processor.execute(0x607B); // LD V0, 0x7B
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF033); // LD B, V0

    EXPECT_EQ(memory.read(0x300), 1);
    EXPECT_EQ(memory.read(0x301), 2);
    EXPECT_EQ(memory.read(0x302), 3);
}

TEST_F(ProcessorTest, ldIReg_LoadsRegistersIntoMemory)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02
    processor.execute(0x6203); // LD V2, 0x03
    processor.execute(0x6304); // LD V3, 0x04
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF255); // LD [I], V2

    EXPECT_EQ(memory.read(0x300), 0x01);
    EXPECT_EQ(memory.read(0x301), 0x02);
    EXPECT_EQ(memory.read(0x302), 0x03);
    EXPECT_EQ(memory.read(0x303), 0x0);
}

TEST_F(ProcessorTest, ldIReg_IndexIncrementDisabled_DoesNotIncrementI)
{
    processor.execute(0x6001); // LD V0, 0x01
    processor.execute(0x6102); // LD V1, 0x02
    processor.execute(0x6203); // LD V2, 0x03
    processor.execute(0x6304); // LD V3, 0x04
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF255); // LD [I], V2

    EXPECT_EQ(processor.getRegisterI(), 0x300);
}

TEST_F(ProcessorTest, ldIReg_IndexIncrementEnabled_IncrementsI)
{
    Processor processorWithIndexIncrement(memory, display, keyboard, Quirks{ .indexIncrement = true }, 0x200);
    processorWithIndexIncrement.execute(0x6001); // LD V0, 0x01
    processorWithIndexIncrement.execute(0x6102); // LD V1, 0x02
    processorWithIndexIncrement.execute(0x6203); // LD V2, 0x03
    processorWithIndexIncrement.execute(0x6304); // LD V3, 0x04
    processorWithIndexIncrement.execute(0xA300); // LD I, 0x300

    processorWithIndexIncrement.execute(0xF255); // LD [I], V2

    EXPECT_EQ(processorWithIndexIncrement.getRegisterI(), 0x304);
}

TEST_F(ProcessorTest, ldRegI_LoadsMemoryIntoRegisters)
{
    memory.write(0x300, 0x01);
    memory.write(0x301, 0x02);
    memory.write(0x302, 0x03);
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF265); // LD V2, [I]

    EXPECT_EQ(processor.getRegistersV()[0], 0x01);
    EXPECT_EQ(processor.getRegistersV()[1], 0x02);
    EXPECT_EQ(processor.getRegistersV()[2], 0x03);
    EXPECT_EQ(processor.getRegistersV()[3], 0x00);
}

TEST_F(ProcessorTest, ldRegI_IndexIncrementDisabled_DoesNotIncrementI)
{
    memory.write(0x300, 0x01);
    memory.write(0x301, 0x02);
    memory.write(0x302, 0x03);
    processor.execute(0xA300); // LD I, 0x300

    processor.execute(0xF265); // LD V2, [I]

    EXPECT_EQ(processor.getRegisterI(), 0x300);
}

TEST_F(ProcessorTest, ldRegI_IndexIncrementEnabled_IncrementsI)
{
    Processor processorWithIndexIncrement(memory, display, keyboard, Quirks{ .indexIncrement = true }, 0x200);
    memory.write(0x300, 0x01);
    memory.write(0x301, 0x02);
    memory.write(0x302, 0x03);
    processorWithIndexIncrement.execute(0xA300); // LD I, 0x300

    processorWithIndexIncrement.execute(0xF265); // LD V2, [I]

    EXPECT_EQ(processorWithIndexIncrement.getRegisterI(), 0x304);
}