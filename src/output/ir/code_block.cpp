#include "code_block.h"

const std::vector<codesh::output::ir::instruction> &codesh::output::ir::code_block::get_instructions() const
{
    return instructions;
}

void codesh::output::ir::code_block::add_instruction(instruction instruction)
{
    instructions.emplace_back(std::move(instruction));
}
