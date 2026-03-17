#include "array_access_ast_node.h"

codesh::ast::op::array_access_ast_node::array_access_ast_node(
        const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> array,
        std::unique_ptr<value_ast_node> index) :
    value_ast_node(code_position),
    array(std::move(array)),
    index(std::move(index))
{
}

codesh::ast::var_reference::value_ast_node& codesh::ast::op::array_access_ast_node::get_array() const
{
    return *array;
}

codesh::ast::var_reference::value_ast_node& codesh::ast::op::array_access_ast_node::get_index() const
{
    return *index;
}

codesh::ast::type::type_ast_node* codesh::ast::op::array_access_ast_node::get_type() const
{
    return nullptr;
}

void codesh::ast::op::array_access_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
}
