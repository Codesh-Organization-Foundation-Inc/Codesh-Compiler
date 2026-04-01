#include "array_load_instruction.h"
#include "impl/instruction_container.h"

codesh::output::ir::array_load_instruction::array_load_instruction(const instruction_type type) :
    type(type)
{
}

size_t codesh::output::ir::array_load_instruction::size() const
{
    return 1;
}

void codesh::output::ir::array_load_instruction::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(
        *opcode::I_ARRAY_LOAD + *type,
        -1
    );
}
