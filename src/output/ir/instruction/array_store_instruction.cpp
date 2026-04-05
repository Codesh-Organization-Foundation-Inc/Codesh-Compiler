#include "array_store_instruction.h"
#include "impl/instruction_container.h"

codesh::output::ir::array_store_instruction::array_store_instruction(const instruction_type type) :
    type(type)
{
}

size_t codesh::output::ir::array_store_instruction::size() const
{
    return 1;
}

void codesh::output::ir::array_store_instruction::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(
        *opcode::I_ARRAY_STORE + *type,
        -3
    );
}
