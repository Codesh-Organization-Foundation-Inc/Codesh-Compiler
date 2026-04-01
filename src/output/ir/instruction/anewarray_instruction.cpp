#include "anewarray_instruction.h"

#include "impl/instruction_container.h"
#include "impl/opcode.h"
#include "util.h"

codesh::output::ir::anewarray_instruction::anewarray_instruction(const int class_cp_index) :
    class_cp_index(class_cp_index)
{
}

size_t codesh::output::ir::anewarray_instruction::size() const
{
    return 3;
}

void codesh::output::ir::anewarray_instruction::emit(std::vector<instruction_container> &collector) const
{
    std::vector<unsigned char> opcodes(3);

    opcodes[0] = *opcode::ANEWARRAY;
    util::put_int_bytes(opcodes.data() + 1, 2, class_cp_index);

    collector.emplace_back(std::move(opcodes), 0);
}
