cmake_minimum_required(VERSION 3.24)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core Lib
add_library(
    core_lib
    src/memory/memory.cpp
    src/processor/processor.cpp
    src/display/display.cpp
    src/keyboard/keyboard.cpp
)
target_include_directories(core_lib PUBLIC src)

# Executable
add_executable(
    chip8 
    src/main.cpp
)
target_link_libraries(chip8 PRIVATE core_lib)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    run_tests
    tests/display/displayTest.cpp
    tests/memory/memoryTest.cpp
    tests/keyboard/keyboardTest.cpp
)

target_link_libraries(run_tests PRIVATE 
    core_lib
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)