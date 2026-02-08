#pragma once

#include "goto_instruction.h"
#include "impl/opcode.h"

namespace codesh::output::ir
{

class if_instruction final : public goto_instruction
{
    if_type type;

public:
    if_instruction(if_type type, int jump_offset);

    void emit(std::vector<instruction_container> &collector) const override;
};

}
