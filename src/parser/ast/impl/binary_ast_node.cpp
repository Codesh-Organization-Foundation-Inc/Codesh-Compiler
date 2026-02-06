#include "binary_ast_node.h"

#include "parser/ast/type/primitive_type_ast_node.h"
#include "semantic_analyzer/util.h"

codesh::ast::impl::binary_ast_node::binary_ast_node(const blasphemy::code_position code_position,
        std::unique_ptr<value_ast_node> left, std::unique_ptr<value_ast_node> right) :
    operator_ast_node(code_position)
{
    children[0] = std::move(left);
    children[1] = std::move(right);
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::binary_ast_node::get_left() const
{
    return *children[0];
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::binary_ast_node::get_right() const
{
    return *children[1];
}

bool codesh::ast::impl::binary_ast_node::is_value_valid() const
{
    return semantic_analyzer::util::are_types_compatible(*get_left().get_type(), *get_right().get_type());
}

codesh::ast::type::type_ast_node *codesh::ast::impl::binary_ast_node::get_type() const
{
    //TODO: Add auto casting logic
    // For now assume the same type
    return get_left().get_type();
}
