#include "instruction.h"

#include "../../util.h"
#include "../jvm_target/constant_pool.h"

#include <limits>
#include <stdexcept>

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

codesh::output::ir::instruction::~instruction() = default;

codesh::output::ir::simple_instruction::simple_instruction(const opcode _opcode, const int stack_delta) :
    _opcode(_opcode),
    stack_delta(stack_delta)
{
}

codesh::output::ir::opcode codesh::output::ir::simple_instruction::get_opcode() const
{
    return _opcode;
}

int codesh::output::ir::simple_instruction::get_stack_delta() const
{
    return stack_delta;
}

void codesh::output::ir::simple_instruction::emit(std::list<instruction_container> &collector) const
{
    collector.emplace_back(*_opcode, stack_delta);
}

codesh::output::ir::typed_instruction::typed_instruction(const instruction_type type) :
    type(type)
{
}

codesh::output::ir::instruction_type codesh::output::ir::typed_instruction::get_instruction_type() const
{
    return type;
}

codesh::output::ir::nop_instruction::nop_instruction() : simple_instruction(opcode::NOP, 0)
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

void codesh::output::ir::load_instruction::emit(std::list<instruction_container> &collector) const
{
    if (lvt_index <= 3)
    {
        collector.emplace_back(
            *opcode::I_LOAD_0
                + *get_instruction_type() * CONSTANT_INDEXES_COUNT
                + lvt_index,
            1
        );
    }
    else
    {
        collector.emplace_back(
            std::vector {
                static_cast<unsigned char>(*opcode::I_LOAD + *get_instruction_type()),
                lvt_index,
            },
            1
        );
    }
}

codesh::output::ir::return_instruction::return_instruction() :
    simple_instruction(opcode::RETURN, 0)
{
}

codesh::output::ir::invoke_instruction::invoke_instruction(const invokation_type type, const int method_cp_index,
        const int parameters_count) :
    type(type),
    method_cp_index(method_cp_index),
    parameters_count(parameters_count)
{
}

void codesh::output::ir::invoke_instruction::emit(std::list<instruction_container> &collector) const
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

int codesh::output::ir::invoke_instruction::get_method_cp_index() const
{
    return method_cp_index;
}

codesh::output::ir::load_int_constant_instruction::load_int_constant_instruction(const int constant,
        const jvm_target::constant_pool &fallback_constant_pool) :
    constant(constant),
    fallback_constant_pool(fallback_constant_pool)
{
}

void codesh::output::ir::load_int_constant_instruction::emit(std::list<instruction_container> &collector) const
{
    if (-1 <= constant && constant <= 5)
    {
        collector.emplace_back(
            *opcode::I_CONST_M1 + (constant + 1),
            1
        );

        return;
    }

    if (std::numeric_limits<int8_t>::min() <= constant && constant <= std::numeric_limits<int8_t>::max())
    {
        collector.emplace_back(
            std::vector {
                *opcode::B_IPUSH,
                static_cast<unsigned char>(constant)
            },
            1
        );
    }
    else if (std::numeric_limits<int16_t>::min() <= constant && constant <= std::numeric_limits<int16_t>::max())
    {
        std::vector<unsigned char> opcodes(3);
        opcodes[0] = *opcode::S_IPUSH;
        util::put_int_bytes(opcodes.data() + 1, 2, constant);

        collector.emplace_back(std::move(opcodes), 1);
    }
    else
    {
        // If the number is greater than int16, then it is saved in the constant pool.
        load_constant_pool_instruction(fallback_constant_pool.get_integer_index(constant))
            .emit(collector);
    }
}

codesh::output::ir::load_constant_pool_instruction::load_constant_pool_instruction(const int constant_pool_index) :
    constant_pool_index(constant_pool_index)
{
}

void codesh::output::ir::load_constant_pool_instruction::emit(std::list<instruction_container> &collector) const
{
    collector.emplace_back(
        std::vector {
            *opcode::LDC,
            static_cast<unsigned char>(constant_pool_index)
        },
        1
    );
}

codesh::output::ir::get_static_instruction::get_static_instruction(int constant_pool_index) :
    constant_pool_index(constant_pool_index)
{
}

void codesh::output::ir::get_static_instruction::emit(std::list<instruction_container> &collector) const
{
    std::vector<unsigned char> opcodes(3);

    opcodes[0] = *opcode::GET_STATIC;
    util::put_int_bytes(opcodes.data() + 1, 2, constant_pool_index);

    collector.emplace_back(std::move(opcodes), 1);
}
