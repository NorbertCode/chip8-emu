add_library(
    chip8_core
    memory/memory.cpp
    memory/storage.cpp
    processor/processor.cpp
    peripherals/display.cpp
    peripherals/keyboard.cpp
    chip8.cpp
)

target_link_libraries(chip8_core PRIVATE warnings)

target_include_directories(chip8_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})