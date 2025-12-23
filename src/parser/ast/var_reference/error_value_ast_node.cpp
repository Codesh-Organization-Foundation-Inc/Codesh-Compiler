#include "error_value_ast_node.h"

codesh::ast::type::type_ast_node *codesh::ast::var_reference::error_value_ast_node::get_type() const
{
    return nullptr;
}

void codesh::ast::var_reference::error_value_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    throw std::runtime_error("Attempted to emit IR for an errored value");
}
