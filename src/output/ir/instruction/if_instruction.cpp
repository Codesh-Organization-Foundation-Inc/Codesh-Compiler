#include "if_instruction.h"
#include "impl/instruction_container.h"

codesh::output::ir::if_instruction::if_instruction(const if_type type, const int jump_offset) :
    goto_instruction(jump_offset),
    type(type)
{
}

void codesh::output::ir::if_instruction::emit(std::vector<instruction_container> &collector) const
{
    goto_instruction::emit(collector);

    collector.back().opcodes[0] = *opcode::IF_ZERO + *type;
    collector.back().size_delta = type >= if_type::ARE_INTS_EQUAL
        ? -2
        : -1;
}
