add_library(
    chip8_front
    application/application.cpp
    application/renderer.cpp
    application/input.cpp
)

find_package(SDL2 REQUIRED)
target_link_libraries(chip8_front PRIVATE SDL2::SDL2)

target_link_libraries(chip8_front PUBLIC chip8_core)
target_include_directories(chip8_front PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})