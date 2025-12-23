#include "binary_ast_node.h"

codesh::ast::impl::binary_ast_node::binary_ast_node(std::unique_ptr<ir_emitting_ast_node> left,
                                                       std::unique_ptr<ir_emitting_ast_node> right) :
    left(std::move(left)), right(std::move(right))
{
}

codesh::ast::impl::ir_emitting_ast_node *codesh::ast::impl::binary_ast_node::get_left() const
{
    return this->left.get();
}
codesh::ast::impl::ir_emitting_ast_node *codesh::ast::impl::binary_ast_node::get_right() const
{
    return this->right.get();
}

void codesh::ast::impl::binary_ast_node::set_left(std::unique_ptr<ir_emitting_ast_node> node)
{
    this->left = std::move(node);
}
void codesh::ast::impl::binary_ast_node::set_right(std::unique_ptr<ir_emitting_ast_node> node)
{
    this->right = std::move(node);
}
