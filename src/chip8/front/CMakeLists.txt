add_library(
    chip8_front
    application.cpp
    loader.cpp
    components/renderer.cpp
    components/input.cpp
    components/audio.cpp
    debugger/debugger.cpp
    debugger/debuggerBuilder.cpp
    debugger/widgets/demoWidget.cpp
    debugger/widgets/memoryViewerWidget.cpp
    debugger/widgets/disassemblyViewerWidget.cpp
    debugger/widgets/stackViewerWidget.cpp
    debugger/widgets/viewportWidget.cpp
    debugger/widgets/registersViewWidget.cpp
    debugger/widgets/spritePreviewWidget.cpp
    debugger/widgets/flowControlWidget.cpp
    debugger/widgets/breakpointsWidget.cpp
    debugger/widgets/configurationWidget.cpp
)

if (NOT EMSCRIPTEN)
    find_package(SDL2 REQUIRED CONFIG)
    target_link_libraries(chip8_front PUBLIC SDL2::SDL2)
else ()
    target_compile_options(chip8_front PUBLIC "-sUSE_SDL=2")
    target_link_options(chip8_front PUBLIC "-sUSE_SDL=2")
endif ()

target_link_libraries(chip8_front PUBLIC 
    chip8_core
    disassembler_core
    tomlplusplus::tomlplusplus
    imgui
)

target_include_directories(chip8_front PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})