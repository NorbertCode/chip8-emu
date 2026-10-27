#pragma once
#include <string>

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
        static Args parseArgs(const std::string& name, int argc, char** argv);
    };
}