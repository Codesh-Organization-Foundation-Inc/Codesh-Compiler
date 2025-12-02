#pragma once

#include "instruction.h"

namespace codesh::output::ir
{

class code_block
{
    std::vector<instruction> instructions;

public:
    [[nodiscard]] const std::vector<instruction> &get_instructions() const;
    void add_instruction(instruction instruction);
};

}
