add_subdirectory(core)
add_subdirectory(front)

if (EMSCRIPTEN)
    add_subdirectory(platform/wasm)
else ()
    add_subdirectory(platform/desktop)
endif ()