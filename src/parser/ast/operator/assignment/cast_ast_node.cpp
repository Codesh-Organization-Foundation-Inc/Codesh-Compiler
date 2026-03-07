#include "cast_ast_node.h"

codesh::ast::op::assignment::cast_ast_node::cast_ast_node(blasphemy::code_position code_position,
        std::unique_ptr<value_ast_node> left, type::type_ast_node* right) :
    value_ast_node(code_position),
    left(std::move(left)),
    right(right)
{
}

const codesh::ast::var_reference::value_ast_node& codesh::ast::op::assignment::cast_ast_node::get_left() const
{
    return *left;
}

codesh::ast::type::type_ast_node* codesh::ast::op::assignment::cast_ast_node::get_right() const
{
    return right;
}

codesh::ast::type::type_ast_node* codesh::ast::op::assignment::cast_ast_node::get_type() const
{
    return right;
}

void codesh::ast::op::assignment::cast_ast_node::emit_ir(output::ir::code_block& containing_block,
    const semantic_analyzer::symbol_table& symbol_table,
    const type_decl::type_declaration_ast_node& containing_type_decl) const
{
}

