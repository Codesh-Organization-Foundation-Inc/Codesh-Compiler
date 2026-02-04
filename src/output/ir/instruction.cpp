#include "instruction.h"

#include "parser/ast/method/method_scope_ast_node.h"
#include "util.h"
#include "output/jvm_target/constant_pool.h"

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

codesh::output::ir::simple_instruction::simple_instruction(opcode _opcode, int stack_delta, const size_t _size) :
    _opcode(_opcode),
    stack_delta(stack_delta),
    _size(_size)
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

void codesh::output::ir::simple_instruction::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(*_opcode, stack_delta);
}

size_t codesh::output::ir::simple_instruction::size() const
{
    return _size;
}

codesh::output::ir::typed_instruction::typed_instruction(const instruction_type type, const unsigned char index) :
    type(type),
    index(index)
{
}

size_t codesh::output::ir::typed_instruction::size() const
{
    return index <= 3 ? 1 : 2;
}

void codesh::output::ir::typed_instruction::emit(std::vector<instruction_container> &collector) const
{
    if (index <= 3)
    {
        const unsigned char first_non_generic = *first_generic() + CONSTANT_INDEXES_COUNT + 1;

        collector.emplace_back(
            first_non_generic
                + *type * CONSTANT_INDEXES_COUNT
                + index,
            1
        );
    }
    else
    {
        collector.emplace_back(
            std::vector {
                static_cast<unsigned char>(*first_generic() + *type),
                index
            },
            1
        );
    }
}

codesh::output::ir::nop_instruction::nop_instruction() : simple_instruction(opcode::NOP, 0, 1)
{
}

codesh::output::ir::opcode codesh::output::ir::load_instruction::first_generic() const
{
    return opcode::I_LOAD;
}

codesh::output::ir::load_instruction::load_instruction(const instruction_type type, const unsigned char local_var_index) :
    typed_instruction(type, local_var_index)
{
}

codesh::output::ir::return_instruction::return_instruction() :
    simple_instruction(opcode::RETURN, 0, 1)
{
}

codesh::output::ir::return_instruction::return_instruction(const instruction_type type) :
    simple_instruction(static_cast<opcode>(*opcode::I_RETURN + *type), -1, 1)
{
}

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

codesh::output::ir::load_int_constant_instruction::load_int_constant_instruction(const int constant,
        const std::optional<int> constant_cpi) :
    constant(constant),
    constant_cpi(constant_cpi)
{
}

size_t codesh::output::ir::load_int_constant_instruction::size() const
{
    if (constant_cpi.has_value())
    {
        // Constant size of load_constant_pool_instruction
        return 2;
    }


    if (-1 <= constant && constant <= 5)
    {
        return 1;
    }

    if (std::numeric_limits<int8_t>::min() <= constant && constant <= std::numeric_limits<int8_t>::max())
    {
        return 2;
    }
    if (std::numeric_limits<int16_t>::min() <= constant && constant <= std::numeric_limits<int16_t>::max())
    {
        return 3;
    }

    // If the number is greater than int16, then it must be saved in the constant pool.
    throw std::runtime_error("Attempted to load a number greater than int16 without a constant pool entry");
}

void codesh::output::ir::load_int_constant_instruction::emit(std::vector<instruction_container> &collector) const
{
    if (constant_cpi.has_value())
    {
        load_constant_pool_instruction(constant_cpi.value())
            .emit(collector);

        return;
    }


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
        // If the number is greater than int16, then it must be saved in the constant pool.
        throw std::runtime_error("Attempted to load a number greater than int16 without a constant pool entry");
    }
}

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

codesh::output::ir::opcode codesh::output::ir::store_in_local_var_instruction::first_generic() const
{
    return opcode::I_STORE;
}

codesh::output::ir::store_in_local_var_instruction::store_in_local_var_instruction(const instruction_type type,
                                                                                   const int local_var_index) :
    typed_instruction(type, local_var_index)
{
}

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

codesh::output::ir::if_instruction::if_instruction(const if_type type, const int jump_offset) :
    goto_instruction(jump_offset),
    type(type)
{
}

void codesh::output::ir::if_instruction::emit(std::vector<instruction_container> &collector) const
{
    goto_instruction::emit(collector);

    collector.back().opcodes[0] = *opcode::IF_ZERO + *type;
    collector.back().size_delta = type >= if_type::ARE_INTS_EQUAL
        ? -2
        : -1;
}

void codesh::output::ir::marker_instruction::emit(std::vector<instruction_container> &collector) const
{
}

size_t codesh::output::ir::marker_instruction::size() const
{
    return 0;
}

codesh::output::ir::stack_size_delta_marker::stack_size_delta_marker(const size_t size_delta) : size_delta(size_delta)
{
}

void codesh::output::ir::stack_size_delta_marker::emit(std::vector<instruction_container> &collector) const
{
    collector.emplace_back(std::vector<unsigned char>{}, static_cast<int>(size_delta));
}

codesh::output::ir::scope_marker::scope_marker(const ast::method::method_scope_ast_node &scope) :
    scope(scope)
{
}

const codesh::ast::method::method_scope_ast_node &codesh::output::ir::scope_marker::get_scope() const
{
    return scope;
}

size_t codesh::output::ir::scope_marker::get_bytecode_position() const
{
    return bytecode_position;
}

void codesh::output::ir::scope_marker::set_bytecode_position(const size_t pos)
{
    bytecode_position = pos;
}

codesh::output::ir::scope_begin_marker::scope_begin_marker(const ast::method::method_scope_ast_node &scope) :
        scope_marker(scope)
{
}

codesh::output::ir::scope_end_marker::scope_end_marker(const ast::method::method_scope_ast_node &scope) :
        scope_marker(scope)
{
}
