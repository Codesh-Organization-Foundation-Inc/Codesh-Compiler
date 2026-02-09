#pragma once

#include "evaluable_ast_node.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/load_constant_pool_instruction.h"
#include "output/ir/instruction/load_int_constant_instruction.h"
#include "output/ir/instruction/load_wide_constant_pool_instruction.h"
#include "output/ir/util.h"
#include "output/jvm_target/constant_pool.h"

template <typename T>
codesh::ast::var_reference::evaluable_ast_node<T>::evaluable_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<type::type_ast_node> type, T value) :
    value_ast_node(code_position),
    type(std::move(type)),
    value(std::move(value))
{
}

template <typename T>
codesh::ast::type::type_ast_node *codesh::ast::var_reference::evaluable_ast_node<T>::get_type() const
{
    return type.get();
}

template <typename T>
T codesh::ast::var_reference::evaluable_ast_node<T>::get_value() const
{
    return value;
}

template <typename T>
void codesh::ast::var_reference::evaluable_ast_node<T>::set_value(T value)
{
    this->value = std::move(value);
}

template <typename T>
void codesh::ast::var_reference::evaluable_ast_node<T>::emit_constants(
    const compilation_unit_ast_node &root_node, output::jvm_target::constant_pool &constant_pool)
{
    if constexpr (std::is_same_v<T, int>)
    {
        //FIXME: Currently, if there exists a big integer made for subtraction, it will also save a constant for the
        // unnecessary positive value of it.
        value_cpi = output::ir::util::goc_big_int_value(*this, constant_pool, output::ir::operator_type::ASSIGN);
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        value_cpi = constant_pool.goc_float_info(value);
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        value_cpi = constant_pool.goc_double_info(value);
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        value_cpi = constant_pool.goc_string_info(constant_pool.goc_utf8_info(value));
    }
}

template <typename T>
void codesh::ast::var_reference::evaluable_ast_node<T>::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &) const
{
    if constexpr (std::is_same_v<T, int> || std::is_same_v<T, bool> || std::is_same_v<T, char>)
    {
        containing_block.add_instruction(std::make_unique<output::ir::load_int_constant_instruction>(value, value_cpi));
    }
    else if constexpr (std::is_same_v<T, double>)
    {
        containing_block.add_instruction(std::make_unique<output::ir::load_wide_constant_pool_instruction>(*value_cpi));
    }
    else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, float>)
    {
        containing_block.add_instruction(std::make_unique<output::ir::load_constant_pool_instruction>(*value_cpi));
    }
}
