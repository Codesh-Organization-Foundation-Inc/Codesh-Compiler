#include "assignment_instruction.h"
#include "load_instruction.h"
#include "operator_instruction.h"
#include "store_in_local_var_instruction.h"

codesh::output::ir::assignment_instruction::assignment_instruction(const instruction_type type,
        const operator_type op_type, const int target_lvt_index) :
    target_lvt_index(target_lvt_index),
    op_type(op_type),
    type(type)
{
}

size_t codesh::output::ir::assignment_instruction::size() const
{
    return
        load_instruction(type, target_lvt_index).size()
        + rhs_size()
        + operator_instruction(type, op_type).size()
        + store_in_local_var_instruction(type, target_lvt_index).size();
}

void codesh::output::ir::assignment_instruction::emit(std::vector<instruction_container> &collector) const
{
    // Load target
    load_instruction(type, target_lvt_index).emit(collector);
    // Load RHS
    emit_rhs(collector);

    // Apply operator
    operator_instruction(type, op_type).emit(collector);

    // Store result in target
    store_in_local_var_instruction(type, target_lvt_index).emit(collector);
}
