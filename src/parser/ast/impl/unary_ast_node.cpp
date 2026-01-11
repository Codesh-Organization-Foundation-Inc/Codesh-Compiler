#include "unary_ast_node.h"

codesh::ast::impl::unary_ast_node::unary_ast_node(std::unique_ptr<value_ast_node> child) :
    child(std::move(child))
{
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::unary_ast_node::get_child() const
{
    return *child;
}

codesh::ast::type::type_ast_node *codesh::ast::impl::unary_ast_node::get_type() const
{
    return this->type.get();
}

void codesh::ast::impl::unary_ast_node::set_statement_index(const size_t statement_index)
{
    value_ast_node::set_statement_index(statement_index);
    child->set_statement_index(statement_index);
}

void codesh::ast::impl::unary_ast_node::emit_constants(const compilation_unit_ast_node &root_node,
                                                       output::jvm_target::constant_pool &constant_pool)
{
    if (const auto constant_emitter = dynamic_cast<i_constant_pool_emitter *>(child.get()))
    {
        constant_emitter->emit_constants(root_node, constant_pool);
    }
}
