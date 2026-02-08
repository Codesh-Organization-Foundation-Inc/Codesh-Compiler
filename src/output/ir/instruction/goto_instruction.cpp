#include "goto_instruction.h"
#include "impl/instruction_container.h"
#include "impl/opcode.h"
#include "util.h"

codesh::output::ir::goto_instruction::goto_instruction(const int jump_offset) : jump_offset(jump_offset)
{
}

size_t codesh::output::ir::goto_instruction::size() const
{
    return 3;
}

int codesh::output::ir::goto_instruction::get_jump_offset() const
{
    return jump_offset;
}

void codesh::output::ir::goto_instruction::set_target(const int target)
{
    jump_offset = target - jump_offset;
}

void codesh::output::ir::goto_instruction::emit(std::vector<instruction_container> &collector) const
{
    std::vector<unsigned char> opcodes(3);

    opcodes[0] = *opcode::GOTO;

    util::put_int_bytes(
        opcodes.data() + 1,
        2,
        jump_offset + static_cast<int>(size())
    );

    collector.emplace_back(
        std::move(opcodes),
        0
    );
}
