#include "boolean_binary_ast_node.h"

codesh::ast::op::boolean_binary_ast_node::boolean_binary_ast_node(const blasphemy::code_position &code_position,
                                                 std::unique_ptr<value_ast_node> left,
                                                 std::unique_ptr<value_ast_node> right) :
    binary_ast_node(code_position, std::move(left), std::move(right)),
    type(std::make_unique<type::primitive_type_ast_node>(
        code_position,
        definition::primitive_type::BOOLEAN
    ))
{
}

codesh::ast::type::type_ast_node *codesh::ast::op::boolean_binary_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::op::boolean_binary_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    get_left().emit_ir(containing_block, symbol_table, containing_type_decl);
    get_right().emit_ir(containing_block, symbol_table, containing_type_decl);
}
