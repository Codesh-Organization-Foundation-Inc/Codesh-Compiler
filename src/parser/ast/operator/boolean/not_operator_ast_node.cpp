#include "not_operator_ast_node.h"

codesh::ast::op::not_operator_ast_node::not_operator_ast_node(std::unique_ptr<i_ir_emitter> child)
    : unary_ast_node(std::move(child))
{
}

void codesh::ast::op::not_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}

