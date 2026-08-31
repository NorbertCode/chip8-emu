cmake_minimum_required(VERSION 3.25)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(warnings INTERFACE)
target_compile_options(warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)

add_subdirectory(src)
add_subdirectory(include/imgui SYSTEM)

set_target_properties(imgui PROPERTIES EXPORT_COMPILE_COMMANDS OFF)

enable_testing()
add_subdirectory(tests)