#pragma once
#include "loader.hpp"

namespace chip8::front
{
    class DesktopLoader : public Loader
    {
    public:
        DesktopLoader(const std::string& name, int argc, char** argv);
    };
}