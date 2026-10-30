#pragma once
#include <string>
#include <vector>

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
        static Args parseArgs(const std::string& name, const std::vector<std::string>& args); // Has to take a vector (not span) because of argparse interface
    };
}