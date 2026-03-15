#include "new_instruction.h"

#include "impl/opcode.h"
#include "util.h"

codesh::output::ir::new_instruction::new_instruction(const int class_cp_index) :
    class_cp_index(class_cp_index)
{
}

size_t codesh::output::ir::new_instruction::size() const
{
    return 3;
}

void codesh::output::ir::new_instruction::emit(std::vector<instruction_container> &collector) const
{
    std::vector<unsigned char> opcodes(3);

    opcodes[0] = *opcode::NEW;
    util::put_int_bytes(opcodes.data() + 1, 2, class_cp_index);

    collector.emplace_back(std::move(opcodes), 1);
}
