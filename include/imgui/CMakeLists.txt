add_library(
    imgui
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_stdlib.cpp
    backends/imgui_impl_sdl2.cpp
    backends/imgui_impl_sdlrenderer2.cpp
)

if (NOT EMSCRIPTEN)
    find_package(SDL2 REQUIRED)
    target_link_libraries(imgui PRIVATE SDL2::SDL2)
else ()
    target_compile_options(imgui PUBLIC "-sUSE_SDL=2")
    target_link_options(imgui PUBLIC "-sUSE_SDL=2")
endif ()

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/backends)