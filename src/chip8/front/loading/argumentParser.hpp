#pragma once
#include <string>

namespace chip8::front
{
    struct Args
    {
        std::string romPath;
        std::string configPath;
    };

    class ArgumentParser
    {
    public:
        static Args parseArgs(const std::string& name, int argc, char** argv);
    };
}