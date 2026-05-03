add_subdirectory(chip8_core)
add_subdirectory(chip8_front)

add_executable(
    chip8
    main.cpp
)

target_link_libraries(chip8 PRIVATE chip8_core)
target_link_libraries(chip8 PRIVATE chip8_front)