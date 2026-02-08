#include "instruction_container.h"

codesh::output::ir::instruction_container::instruction_container(std::vector<unsigned char> opcodes,
                                                                 const int size_delta) :
    opcodes(std::move(opcodes)),
    size_delta(size_delta)
{
}

codesh::output::ir::instruction_container::instruction_container(const unsigned char opcode, const int size_delta) :
    instruction_container(std::vector {opcode}, size_delta)
{
}
