#include "disassembler.hpp"
#include "argumentParser.hpp"
#include "resourceLoader.hpp"
#include "configParser.hpp"
#include <cstdint>
#include <format>
#include <iostream>

using namespace disassembler;

int main(int argc, char* argv[])
{
    try
    {
        const front::Args args = front::ArgumentParser::parseArgs("CHIP-DISASM", argc, argv);

        const std::vector<std::uint16_t> rom = front::ResourceLoader::loadRom(args.romPath);
        const std::string configData = front::ResourceLoader::loadConfig(args.configPath);

        const core::DisassemblerConfig config = front::ConfigParser::parseConfig(configData);

        const core::Disassembler disasm(config);
        const std::vector<std::string> output = disasm.disassemble(rom);
        
        if (args.prettify)
        {
            std::cout << std::format("{:<8}{:<8}{:<25}\n", "LINE", "HEX", "INSTRUCTION");
            std::cout << std::format("{:-<36}\n", "");
        }
        
        for (size_t i = 0; i < output.size(); ++i)
        {
            if (args.prettify)
            {
                std::string line = std::format("0x{:04X}", disasm.getProgramStart() + (i * 2));
                std::string hex = std::format("0x{:04X}", rom[i]);

                std::cout << std::format("{:<8}{:<8}{:<25}\n", line, hex, output[i]);
            }
            else
                std::cout << output[i] << '\n';
        }

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}