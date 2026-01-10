#pragma once

#include "output/ir/instruction.h"
#include "output/ir/code_block.h"
#include "output/jvm_target/constant_pool.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"

#include <limits>

template <typename T>
codesh::ast::var_reference::evaluable_ast_node<T>::evaluable_ast_node(std::unique_ptr<type::type_ast_node> type,
        T value) :
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
        // Only save numbers greater than 16 bits
        if (std::numeric_limits<int16_t>::min() > value || value > std::numeric_limits<int16_t>::max())
        {
            value_cpi = constant_pool.goc_integer_info(value);
        }
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
    if constexpr (std::is_same_v<T, int>)
    {
        containing_block.add_instruction(std::make_unique<output::ir::load_int_constant_instruction>(value, value_cpi));
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        containing_block.add_instruction(std::make_unique<output::ir::load_constant_pool_instruction>(*value_cpi));
    }
}
