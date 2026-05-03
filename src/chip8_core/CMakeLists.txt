add_library(
    chip8_core
    memory/memory.cpp
    processor/processor.cpp
    peripherals/display.cpp
    peripherals/keyboard.cpp
    chip8.cpp
)

target_include_directories(chip8_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})