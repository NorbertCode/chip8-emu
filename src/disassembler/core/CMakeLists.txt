add_library(
    disassembler_core
    disassembler.cpp
)

target_link_libraries(disassembler_core PRIVATE warnings)

target_include_directories(disassembler_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})