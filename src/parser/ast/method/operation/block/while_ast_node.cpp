#include "while_ast_node.h"

codesh::ast::method::operation::while_ast_node::while_ast_node(std::unique_ptr<impl::i_ir_emitter> condition)
    : condition(std::move(condition))
{
}

codesh::ast::impl::i_ir_emitter *codesh::ast::method::operation::while_ast_node::get_condition() const
{
    return this->condition.get();
}

void codesh::ast::method::operation::while_ast_node::set_condition(std::unique_ptr<impl::i_ir_emitter> condition)
{
    this->condition = std::move(condition);
}
