#include "code_block.h"

const std::vector<codesh::output::ir_instruction> &codesh::output::code_block::get_instructions() const
{
    return instructions;
}

void codesh::output::code_block::add_instruction(ir_instruction instruction)
{
    instructions.emplace_back(std::move(instruction));
}
