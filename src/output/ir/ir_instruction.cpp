#include "ir_instruction.h"

ir_instruction::ir_instruction(const ir_instruction_type instruction_type,
        std::vector<operand> operands) :
    instruction_type(instruction_type),
    operands(std::move(operands))
{
}

codesh::semantic_analyzer::symbol &ir_instruction::get_address_as_symbol(const size_t address_index) const
{
    return *std::get<codesh::semantic_analyzer::symbol *>(operands[address_index]);
}

codesh::output::jvm_target::defs::cp_info &ir_instruction::get_address_as_cp_info(const size_t address_index)
    const
{
    return *std::get<codesh::output::jvm_target::defs::cp_info *>(operands[address_index]);
}
