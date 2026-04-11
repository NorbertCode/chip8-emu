#pragma once
#include "memory/memory.hpp"
#include "processorState.hpp"

class Processor
{
public:
    Processor(const std::uint16_t startProgramCounter);

    const std::uint16_t fetch(const Memory& memory);
    void execute(const std::uint16_t instruction);

private:
    ProcessorState state;
};