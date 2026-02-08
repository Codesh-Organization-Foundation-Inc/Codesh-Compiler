#include "get_static_instruction.h"

#include "impl/opcode.h"
#include "util.h"

codesh::output::ir::get_static_instruction::get_static_instruction(const int constant_pool_index) :
    constant_pool_index(constant_pool_index)
{
}

size_t codesh::output::ir::get_static_instruction::size() const
{
    return 3;
}

void codesh::output::ir::get_static_instruction::emit(std::vector<instruction_container> &collector) const
{
    std::vector<unsigned char> opcodes(3);

    opcodes[0] = *opcode::GET_STATIC;
    util::put_int_bytes(opcodes.data() + 1, 2, constant_pool_index);

    collector.emplace_back(std::move(opcodes), 1);
}
