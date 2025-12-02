#include "instruction.h"

codesh::output::ir::instruction::instruction(const opcode instruction_type,
        std::vector<operand> operands) :
    instruction_type(instruction_type),
    operands(std::move(operands))
{
}

codesh::semantic_analyzer::symbol &codesh::output::ir::instruction::get_address_as_symbol(const size_t address_index) const
{
    return *std::get<semantic_analyzer::symbol *>(operands[address_index]);
}

codesh::output::jvm_target::defs::cp_info &codesh::output::ir::instruction::get_address_as_cp_info(const size_t address_index)
    const
{
    return *std::get<jvm_target::defs::cp_info *>(operands[address_index]);
}
