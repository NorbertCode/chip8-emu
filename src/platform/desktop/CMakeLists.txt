add_executable(
    chip8
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

target_link_libraries(chip8 PRIVATE
    argparse
    tomlplusplus::tomlplusplus
    chip8_front
    warnings
)

target_include_directories(chip8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
