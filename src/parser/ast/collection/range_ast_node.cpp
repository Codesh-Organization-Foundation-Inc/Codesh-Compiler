#include "range_ast_node.h"

#include "semantic_analyzer/builtins.h"

codesh::ast::collection::range_ast_node::range_ast_node(blasphemy::code_position code_position,
        std::unique_ptr<value_ast_node> from, std::unique_ptr<value_ast_node> to,
        std::unique_ptr<value_ast_node> skip) :
    collection_ast_node(code_position),
    from(std::move(from)),
    to(std::move(to)),
    skip(std::move(skip)),
    type(std::make_unique<type::custom_type_ast_node>(
        code_position,
        definition::fully_qualified_name(code_position, semantic_analyzer::builtins::CLASS_RANGE)
    ))
{
}

void codesh::ast::collection::range_ast_node::set_statement_index(const size_t statement_index)
{
    collection_ast_node::set_statement_index(statement_index);
    from->set_statement_index(statement_index);
    to->set_statement_index(statement_index);
    skip->set_statement_index(statement_index);
}

codesh::ast::var_reference::value_ast_node &codesh::ast::collection::range_ast_node::get_from() const
{
    return *from;
}

codesh::ast::var_reference::value_ast_node &codesh::ast::collection::range_ast_node::get_to() const
{
    return *to;
}

codesh::ast::var_reference::value_ast_node &codesh::ast::collection::range_ast_node::get_skip() const
{
    return *skip;
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
