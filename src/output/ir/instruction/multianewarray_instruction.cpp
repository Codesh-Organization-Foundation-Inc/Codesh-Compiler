#include "multianewarray_instruction.h"

#include "impl/instruction_container.h"
#include "impl/opcode.h"
#include "util.h"

codesh::output::ir::multianewarray_instruction::multianewarray_instruction(
        const int class_cp_index, const unsigned char dimensions) :
    class_cp_index(class_cp_index),
    dimensions(dimensions)
{
}

size_t codesh::output::ir::multianewarray_instruction::size() const
{
    return 4;
}

void codesh::output::ir::multianewarray_instruction::emit(std::vector<instruction_container> &collector) const
{
    std::vector<unsigned char> opcodes(4);

    opcodes[0] = *opcode::MULTIANEWARRAY;
    util::put_int_bytes(opcodes.data() + 1, 2, class_cp_index);
    opcodes[3] = dimensions;

    collector.emplace_back(std::move(opcodes), 1 - static_cast<int>(dimensions));
}
