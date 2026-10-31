#pragma once
#include <string>
#include <vector>

namespace disassembler::front
{
    struct Args
    {
        std::string romPath;
        std::string configPath;
        bool prettify;
    };

    class ArgumentParser
    {
    public:
        static Args parseArgs(const std::string& name, const std::vector<std::string>& args); // Has to take a vector (not span) because of argparse interface
    };
}