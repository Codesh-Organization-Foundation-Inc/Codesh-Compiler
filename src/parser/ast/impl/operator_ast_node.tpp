#pragma once

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