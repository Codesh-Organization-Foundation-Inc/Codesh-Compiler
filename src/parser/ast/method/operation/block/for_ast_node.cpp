#include "for_ast_node.h"

using namespace codesh::ast::block;

for_ast_node::for_ast_node(std::unique_ptr<local_variable_declaration_ast_node> iterator,
                           std::unique_ptr<var_reference::value_ast_node> collection,
                           method::method_scope_ast_node &body_scope) :
    iterator(std::move(iterator)),
    collection(std::move(collection)),
    body_scope(body_scope)
{
}

codesh::ast::local_variable_declaration_ast_node &for_ast_node::get_iterator() const
{
    return *iterator;
}

codesh::ast::var_reference::value_ast_node &for_ast_node::get_collection() const
{
    return *collection;
}

codesh::ast::method::method_scope_ast_node &for_ast_node::get_body_scope() const
{
    return body_scope;
}

void for_ast_node::emit_ir(
    output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl
) const
{
}
