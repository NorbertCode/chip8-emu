add_library(
    chip8_front
    application.cpp
    components/renderer.cpp
    components/input.cpp
    components/audio.cpp
    debugger/debugger.cpp
    debugger/debuggerBuilder.cpp
    debugger/widgets/demoWidget.cpp
)

find_package(SDL2 REQUIRED)

target_link_libraries(chip8_front PRIVATE 
    SDL2::SDL2
    imgui
)

target_link_libraries(chip8_front PUBLIC chip8_core)

target_include_directories(chip8_front PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})