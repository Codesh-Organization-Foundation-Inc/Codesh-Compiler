#include "load_constant_pool_instruction.h"

#include "impl/opcode.h"

codesh::output::ir::load_constant_pool_instruction::load_constant_pool_instruction(const int constant_pool_index) :
    constant_pool_index(constant_pool_index)
{
}

size_t codesh::output::ir::load_constant_pool_instruction::size() const
{
    return 2;
}

void codesh::output::ir::load_constant_pool_instruction::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(
        std::vector {
            *opcode::LDC,
            static_cast<unsigned char>(constant_pool_index)
        },
        1
    );
}
