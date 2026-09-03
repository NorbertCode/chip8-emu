add_executable(
    chip8_wasm
    main.cpp
)

target_link_libraries(chip8_wasm PRIVATE
    chip8_front
    warnings
)

set_target_properties(chip8_wasm PROPERTIES SUFFIX ".html")

target_link_options(chip8_wasm PRIVATE 
    "-sUSE_SDL=2" 
    "-sALLOW_MEMORY_GROWTH=1"
    "-sFORCE_FILESYSTEM=1"
    "-sEXPORTED_RUNTIME_METHODS=ccall"
    "--preload-file=${CMAKE_SOURCE_DIR}/configs/emulator@/configs"
)

target_include_directories(chip8_wasm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
