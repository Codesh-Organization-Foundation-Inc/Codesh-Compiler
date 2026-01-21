#include "range_ast_node.h"

#include "../../../semantic_analyzer/builtins.h"

codesh::ast::collection::range_ast_node::range_ast_node(const int from, const int to, const int skip) :
    from(from),
    to(to),
    skip(skip),
    type(std::make_unique<type::custom_type_ast_node>(
        definition::fully_qualified_class_name(semantic_analyzer::builtins::CLASS_RANGE)
    ))
{
}

int codesh::ast::collection::range_ast_node::get_from() const
{
    return from;
}

int codesh::ast::collection::range_ast_node::get_to() const
{
    return to;
}

int codesh::ast::collection::range_ast_node::get_skip() const
{
    return skip;
}

codesh::ast::type::type_ast_node *codesh::ast::collection::range_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::collection::range_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    //TODO: Emit a new object from Talmud Codesh
}
