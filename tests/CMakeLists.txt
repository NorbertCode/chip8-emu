set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    run_tests
    chip8/displayTest.cpp
    chip8/memoryTest.cpp
    chip8/storageTest.cpp
    chip8/keyboardTest.cpp
    chip8/processorTest.cpp
    disassembler/disassemblerTest.cpp
)

target_link_libraries(
    run_tests PRIVATE 
    chip8_core
    disassembler_core
    gtest
    gtest_main
    gmock
)

include(GoogleTest)
gtest_discover_tests(run_tests)