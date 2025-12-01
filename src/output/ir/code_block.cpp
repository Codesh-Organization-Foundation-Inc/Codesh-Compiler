#include "code_block.h"

const std::vector<codesh::output::ir_instruction> &codesh::output::code_block::get_instructions() const
{
    return instructions;
}

size_t codesh::output::code_block::get_stack_height_at(const size_t instruction_index) const
{
    return instruction_to_stack_height.at(instruction_index);
}

void codesh::output::code_block::add_instruction(ir_instruction instruction, size_t stack_height)
{
    instructions.emplace_back(std::move(instruction));
    instruction_to_stack_height.emplace_back(stack_height);
}
