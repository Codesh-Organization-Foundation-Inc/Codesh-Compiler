#include "invoke_instruction.h"
#include "impl/instruction_container.h"
#include "util.h"

#include <stdexcept>

codesh::output::ir::invoke_instruction::invoke_instruction(const invokation_type type, const int method_cp_index,
        const int parameters_count) :
    type(type),
    method_cp_index(method_cp_index),
    parameters_count(parameters_count)
{
}

size_t codesh::output::ir::invoke_instruction::size() const
{
    return 3;
}

void codesh::output::ir::invoke_instruction::emit(std::vector<instruction_container> &collector) const
{
    opcode instruction;

    switch (type)
    {
        case invokation_type::DYNAMIC: instruction = opcode::INVOKE_SPECIAL; break;
        case invokation_type::INTERFACE: instruction = opcode::INVOKE_INTERFACE; break;
        case invokation_type::SPECIAL: instruction = opcode::INVOKE_SPECIAL; break;
        case invokation_type::STATIC: instruction = opcode::INVOKE_STATIC; break;
        case invokation_type::VIRTUAL: instruction = opcode::INVOKE_VIRTUAL; break;

        default: throw std::runtime_error("Unknown instruction");
    }

    std::vector<unsigned char> opcodes(3);

    opcodes[0] = *instruction;
    util::put_int_bytes(opcodes.data() + 1, 2, method_cp_index);

    collector.emplace_back(std::move(opcodes), -parameters_count);
}
