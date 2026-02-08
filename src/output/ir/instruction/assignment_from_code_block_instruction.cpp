#include "assignment_from_code_block_instruction.h"

#include "parser/ast/var_reference/value_ast_node.h"

codesh::output::ir::assignment_from_code_block_instruction::assignment_from_code_block_instruction(
        const instruction_type type, const operator_type op_type, const int target_lvt_index, code_block _code_block) :
    assignment_instruction(type, op_type, target_lvt_index),
    _code_block(std::move(_code_block))
{
}

void codesh::output::ir::assignment_from_code_block_instruction::emit_rhs(std::vector<instruction_container> &collector)
    const
{
    for (const auto &instruction : _code_block.get_instructions())
    {
        instruction->emit(collector);
    }
}

size_t codesh::output::ir::assignment_from_code_block_instruction::rhs_size() const
{
    return _code_block.size();
}
