#include "desktopLoader.hpp"
#include "disassembler.hpp"
#include <format>
#include <iostream>

using namespace disassembler;

int main(int argc, char* argv[])
{
    front::DesktopLoader loader("CHIP8 DISASM", argc, argv);

    core::Disassembler disasm(loader.getDisassemblerConfig());
    std::vector<std::string> output = disasm.disassemble(loader.getInput());
    
    if (loader.shouldPrettify())
    {
        std::cout << std::format("{:<8}{:<8}{:<25}\n", "LINE", "HEX", "INSTRUCTION");
        std::cout << std::format("{:-<36}\n", "");
    }
    
    for (size_t i = 0; i < output.size(); ++i)
    {
        if (loader.shouldPrettify())
        {
            std::string line = std::format("0x{:04X}", disasm.getProgramStart() + (i * 2));
            std::string hex = std::format("0x{:04X}", loader.getInput()[i]);

            std::cout << std::format("{:<8}{:<8}{:<25}\n", line, hex, output[i]);
        }
        else
            std::cout << output[i] << '\n';
    }

    return 0;
}