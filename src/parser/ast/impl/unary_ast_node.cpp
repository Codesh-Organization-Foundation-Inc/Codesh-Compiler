#include "unary_ast_node.h"

#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::impl::unary_ast_node::unary_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<value_ast_node> child) :
    operator_ast_node(code_position),
    child(std::move(child))
{
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::unary_ast_node::get_child() const
{
    return *child;
}

codesh::ast::type::type_ast_node *codesh::ast::impl::unary_ast_node::get_type() const
{
    return child->get_type();
}

void codesh::ast::impl::unary_ast_node::set_statement_index(const size_t statement_index)
{
    value_ast_node::set_statement_index(statement_index);
    child->set_statement_index(statement_index);
}

bool codesh::ast::impl::unary_ast_node::is_value_valid() const
{
    return true;
}

void codesh::ast::impl::unary_ast_node::emit_ir(output::ir::code_block &containing_block,
                                                const semantic_analyzer::symbol_table &symbol_table,
                                                const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    child->emit_ir(containing_block, symbol_table, containing_type_decl);
}

void codesh::ast::impl::unary_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                       output::jvm_target::constant_pool &constant_pool)
{
    if (const auto constant_emitter = dynamic_cast<i_constant_pool_emitter *>(child.get()))
    {
        constant_emitter->emit_constants(root_node, constant_pool);
    }
}
