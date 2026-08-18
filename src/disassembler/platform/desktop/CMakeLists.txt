add_executable(
    chip8-disasm
    main.cpp
    desktopLoader.cpp
)

include(FetchContent)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
    GIT_TAG        v3.2
    SYSTEM
)
FetchContent_MakeAvailable(argparse)

FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
    SYSTEM
)
FetchContent_MakeAvailable(tomlplusplus)

target_link_libraries(chip8-disasm PRIVATE
    argparse
    tomlplusplus::tomlplusplus
    disassembler_core
    warnings
)

target_include_directories(chip8-disasm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})