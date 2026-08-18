#include "desktopLoader.hpp"
#include "disassembler.hpp"
#include <iostream>

using namespace disassembler;

int main(int argc, char** argv)
{
    front::DesktopLoader loader("CHIP8 DISASM", argc, argv);

    core::Disassembler disasm(loader.getDisassemblerConfig());
    
    for (auto& line : disasm.disassemble(loader.getInput()))
    {
        std::cout << line << "\n";
    }

    return 0;
}