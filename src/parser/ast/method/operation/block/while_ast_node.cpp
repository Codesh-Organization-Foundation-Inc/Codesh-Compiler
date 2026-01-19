#include "while_ast_node.h"

using namespace codesh::ast::block;

while_ast_node::while_ast_node(
    std::unique_ptr<var_reference::value_ast_node> condition,
    method::method_scope_ast_node &body_scope
)
    : condition(std::move(condition)),
      body_scope(body_scope)
{
}

const codesh::ast::var_reference::value_ast_node &while_ast_node::get_condition() const
{
    return *condition;
}

const codesh::ast::method::method_scope_ast_node &while_ast_node::get_body_scope() const
{
    return body_scope;
}

void while_ast_node::emit_ir(
    output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl
) const
{

}
