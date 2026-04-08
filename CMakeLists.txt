cmake_minimum_required(VERSION 3.24)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable
add_executable(chip8 
    src/main.cpp
)