add_executable(
    chip8-disasm
    main.cpp
)

target_link_libraries(chip8-disasm PRIVATE
    disassembler_core
    disassembler_front
    warnings
)

target_include_directories(chip8-disasm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS chip8-disasm DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/configs DESTINATION .)