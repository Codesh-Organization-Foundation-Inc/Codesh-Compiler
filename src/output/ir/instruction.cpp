#include "instruction.h"

#include "../../util.h"

#include <stdexcept>

codesh::output::ir::instruction::instruction(const opcode _opcode) :
    _opcode(_opcode)
{
}

codesh::output::ir::instruction::~instruction() = default;

codesh::output::ir::opcode codesh::output::ir::instruction::get_opcode() const
{
    return _opcode;
}

void codesh::output::ir::instruction::emit(std::list<unsigned char> &collector) const
{
    collector.emplace_back(static_cast<unsigned char>(_opcode));
}

codesh::output::ir::typed_instruction::typed_instruction(const instruction_type type, const opcode _opcode) :
    instruction(_opcode),
    type(type)
{
}

codesh::output::ir::instruction_type codesh::output::ir::typed_instruction::get_instruction_type() const
{
    return type;
}

codesh::output::ir::nop_instruction::nop_instruction() : instruction(opcode::NOP)
{
}

codesh::output::ir::load_instruction::load_instruction(const instruction_type type, const unsigned char lvt_index) :
    typed_instruction(type),
    lvt_index(lvt_index)
{
}

unsigned char codesh::output::ir::load_instruction::get_lvt_index() const
{
    return lvt_index;
}

void codesh::output::ir::load_instruction::emit(std::list<unsigned char> &collector) const
{
    if (lvt_index <= 3)
    {
        collector.emplace_back(
            static_cast<unsigned char>(opcode::I_LOAD_0)
                + static_cast<unsigned char>(get_instruction_type()) * CONSTANT_INDEXES_COUNT
                + lvt_index
        );
    }
    else
    {
        collector.emplace_back(
            static_cast<unsigned char>(opcode::I_LOAD) + static_cast<unsigned char>(get_instruction_type())
        );

        collector.emplace_back(lvt_index);
    }
}

codesh::output::ir::return_instruction::return_instruction() :
    instruction(opcode::RETURN)
{
}

codesh::output::ir::invoke_instruction::invoke_instruction(const invokation_type type, const int method_cp_index) :
    type(type),
    method_cp_index(method_cp_index)
{
}

void codesh::output::ir::invoke_instruction::emit(std::list<unsigned char> &collector) const
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

    collector.push_back(static_cast<unsigned char>(instruction));


    unsigned char buffer[2];
    util::put_int_bytes(buffer, 2, method_cp_index);

    for (unsigned char byte : buffer)
    {
        collector.emplace_back(byte);
    }
}

int codesh::output::ir::invoke_instruction::get_method_cp_index() const
{
    return method_cp_index;
}
