#include "return_ast_node.h"

#include "../../../../output/ir/code_block.h"

void codesh::ast::method::operation::return_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    containing_block.add_instruction(std::make_unique<output::ir::return_instruction>());
}
