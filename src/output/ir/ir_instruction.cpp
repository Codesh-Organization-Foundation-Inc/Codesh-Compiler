#include "ir_instruction.h"

three_address_code_ir::three_address_code_ir(const ir_instruction instruction_type,
        std::array<address, 3> addresses) :
    instruction_type(instruction_type),
    addresses(std::move(addresses))
{
}

codesh::semantic_analyzer::symbol &three_address_code_ir::get_address_as_symbol(const size_t address_index) const
{
    return std::get<codesh::semantic_analyzer::symbol &>(addresses[address_index]);
}

codesh::output::jvm_target::defs::cp_info &three_address_code_ir::get_address_as_cp_info(const size_t address_index)
    const
{
    return std::get<codesh::output::jvm_target::defs::cp_info &>(addresses[address_index]);
}
