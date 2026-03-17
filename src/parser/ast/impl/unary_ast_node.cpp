#include "unary_ast_node.h"

#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::impl::unary_ast_node::unary_ast_node(const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> child) :
    operator_ast_node(code_position)
{
    children[0] = std::move(child);
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::unary_ast_node::get_child() const
{
    return *children[0];
}

codesh::ast::type::type_ast_node *codesh::ast::impl::unary_ast_node::get_type() const
{
    return get_child().get_type();
}

bool codesh::ast::impl::unary_ast_node::is_value_valid() const
{
    return true;
}
