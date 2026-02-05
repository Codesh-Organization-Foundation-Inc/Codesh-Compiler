#pragma once

#include "output/ir/code_block.h"
#include "parser/ast/operator/math/i_math_operator_holder.h"

template <int operands>
void codesh::ast::impl::operator_ast_node<operands>::set_statement_index(
        const size_t statement_index)
{
    value_ast_node::set_statement_index(statement_index);
    for (size_t i = 0; i < operands; ++i)
    {
        children[i]->set_statement_index(statement_index);
    }
}

template <int operands>
void codesh::ast::impl::operator_ast_node<operands>::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    for (size_t i = 0; i < operands; ++i)
    {
        children[i]->emit_ir(containing_block, symbol_table, containing_type_decl);
    }

    if (const auto math_op_holder = dynamic_cast<const op::i_math_operator_holder *>(this))
    {
        containing_block.add_instruction(std::make_unique<output::ir::operator_instruction>(
            get_type()->to_instruction_type(),
            math_op_holder->get_ir_operator_type()
        ));
    }
}

template <int operands>
void codesh::ast::impl::operator_ast_node<operands>::emit_constants(const compilation_unit_ast_node &root_node,
                                                                    output::jvm_target::constant_pool &constant_pool)
{
    for (size_t i = 0; i < operands; ++i)
    {
        if (const auto constant_emitter = dynamic_cast<i_constant_pool_emitter *>(children[i].get()))
        {
            constant_emitter->emit_constants(root_node, constant_pool);
        }
    }
}