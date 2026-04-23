#include "bitwise_not_ast_node.h"

#include "lexer/trie/keywords.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/load_int_constant_instruction.h"
#include "output/ir/instruction/operator_instruction.h"
#include "output/ir/instruction/widening_cast_instruction.h"

codesh::ast::op::bitwise_not_ast_node::bitwise_not_ast_node(const lexer::code_position code_position,
                                                            std::unique_ptr<value_ast_node> child) :
    unary_ast_node(code_position, std::move(child))
{
}

std::string codesh::ast::op::bitwise_not_ast_node::to_pretty_string() const
{
    return lexer::trie::token_to_string(token_group::OPERATOR_BITWISE_NOT);
}

void codesh::ast::op::bitwise_not_ast_node::emit_ir(output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    get_child().emit_ir(containing_block, symbol_table, containing_type_decl);

    containing_block.add_instruction(std::make_unique<output::ir::load_int_constant_instruction>(-1, std::nullopt));

    const auto type = get_type()->to_instruction_type();
    if (type == output::ir::instruction_type::LONG)
    {
        containing_block.add_instruction(std::make_unique<output::ir::widening_cast_instruction>(
            output::ir::instruction_type::INT, output::ir::instruction_type::LONG
        ));
    }

    containing_block.add_instruction(std::make_unique<output::ir::operator_instruction>(type, output::ir::operator_type::XOR));
}
