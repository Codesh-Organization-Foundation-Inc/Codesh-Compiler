#include "array_length_ast_node.h"
#include "defenition/primitive_type.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/array_length_instruction.h"

codesh::ast::method::operation::array_length_ast_node::array_length_ast_node(
        const lexer::code_position code_position,
        std::unique_ptr<value_ast_node> child) :
    unary_ast_node(code_position, std::move(child)),
    integer_type(std::make_unique<type::primitive_type_ast_node>(code_position, definition::primitive_type::INTEGER))
{
}

std::string codesh::ast::method::operation::array_length_ast_node::to_pretty_string() const
{
    return "אֹרֶךְ־מִסְדָּר";
}

codesh::ast::type::type_ast_node *codesh::ast::method::operation::array_length_ast_node::get_type() const
{
    return integer_type.get();
}

void codesh::ast::method::operation::array_length_ast_node::emit_ir(
        output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    get_child().emit_ir(containing_block, symbol_table, containing_type_decl);
    containing_block.add_instruction(std::make_unique<output::ir::array_length_instruction>());
}
