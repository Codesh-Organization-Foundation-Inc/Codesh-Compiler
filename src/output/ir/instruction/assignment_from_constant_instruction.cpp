#include "assignment_from_constant_instruction.h"
#include "load_int_constant_instruction.h"

codesh::output::ir::assignment_from_constant_instruction::assignment_from_constant_instruction(
        const instruction_type type, const operator_type op_type, const int target_lvt_index,
        const int constant_addition, const std::optional<int> constant_addition_cpi) :
    assignment_instruction(type, op_type, target_lvt_index),
    constant_addition(constant_addition),
    constant_addition_cpi(constant_addition_cpi)
{
}

void codesh::output::ir::assignment_from_constant_instruction::emit_rhs(
    std::vector<instruction_container> &collector) const
{
    load_int_constant_instruction(constant_addition, constant_addition_cpi)
        .emit(collector);
}

size_t codesh::output::ir::assignment_from_constant_instruction::rhs_size() const
{
    return load_int_constant_instruction(constant_addition, constant_addition_cpi)
        .size();
}
