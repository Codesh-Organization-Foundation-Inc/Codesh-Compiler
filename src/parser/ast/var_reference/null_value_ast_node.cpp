#include "null_value_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/aconst_null_instruction.h"

codesh::ast::var_reference::null_value_ast_node::null_value_ast_node(const lexer::code_position code_position) :
    value_ast_node(code_position),
    null_type(code_position)
{
}

codesh::ast::type::type_ast_node *codesh::ast::var_reference::null_value_ast_node::get_type() const
{
    return const_cast<type::null_type_ast_node *>(&null_type);
}

void codesh::ast::var_reference::null_value_ast_node::emit_ir(
    output::ir::code_block &containing_block,
    const semantic_analyzer::symbol_table &,
    const type_decl::type_declaration_ast_node &) const
{
    containing_block.add_instruction(std::make_unique<output::ir::aconst_null_instruction>());
}
