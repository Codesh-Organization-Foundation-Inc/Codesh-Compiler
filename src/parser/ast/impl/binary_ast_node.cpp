#include "binary_ast_node.h"

codesh::ast::impl::binary_ast_node::binary_ast_node(std::unique_ptr<value_ast_node> left,
                                                       std::unique_ptr<value_ast_node> right) :
    left(std::move(left)), right(std::move(right))
{
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::binary_ast_node::get_left() const
{
    return *this->left;
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::binary_ast_node::get_right() const
{
    return *this->right;
}

void codesh::ast::impl::binary_ast_node::set_statement_index(const size_t statement_index)
{
    value_ast_node::set_statement_index(statement_index);
    left->set_statement_index(statement_index);
    right->set_statement_index(statement_index);
}

codesh::ast::type::type_ast_node *codesh::ast::impl::binary_ast_node::get_type() const
{
    return this->type.get();
}
void codesh::ast::impl::binary_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                        output::jvm_target::constant_pool &constant_pool)
{
    if (const auto constant_emitter = dynamic_cast<i_constant_pool_emitter *>(left.get()))
    {
        constant_emitter->emit_constants(root_node, constant_pool);
    }
    if (const auto constant_emitter = dynamic_cast<i_constant_pool_emitter *>(right.get()))
    {
        constant_emitter->emit_constants(root_node, constant_pool);
    }
}
