#include "unary_ast_node.h"

codesh::ast::impl::unary_ast_node::unary_ast_node(std::unique_ptr<i_ir_emitter> child) :
    child(std::move(child))
{
}

codesh::ast::impl::i_ir_emitter *codesh::ast::impl::unary_ast_node::get_child() const
{
    return child.get();
}