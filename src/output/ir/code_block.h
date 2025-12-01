#pragma once

#include "ir_instruction.h"

namespace codesh::output
{

class code_block
{
    std::vector<ir_instruction> instructions;

public:
    [[nodiscard]] const std::vector<ir_instruction> &get_instructions() const;
    void add_instruction(ir_instruction instruction);
};

}
