#include "assign_operator_ast_node.h"

#include "lexer/trie/keywords.h"
#include "output/ir/code_block.h"
#include "output/ir/condition_block_builder.h"
#include "parser/ast/operator/boolean/boolean_binary_ast_node.h"
#include "parser/ast/operator/boolean/not_operator_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

codesh::ast::op::assignment::assign_operator_ast_node::assign_operator_ast_node(
        const blasphemy::code_position code_position, std::unique_ptr<variable_reference_ast_node> left,
        std::unique_ptr<value_ast_node> right) :
    assignment_operator_ast_node(code_position, std::move(left), std::move(right))
{
}

std::string codesh::ast::op::assignment::assign_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_REPLACE);
}

std::optional<codesh::output::ir::operator_type> codesh::ast::op::assignment::assign_operator_ast_node::
    get_operator_type() const
{
    return std::nullopt;
}

void codesh::ast::op::assignment::assign_operator_ast_node::emit_ir(
    output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
    const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    if (dynamic_cast<const boolean_binary_ast_node *>(&get_right())
        || dynamic_cast<const not_operator_ast_node *>(&get_right()))
    {
        containing_block.consume_code_block(output::ir::build_boolean_value_block(
            get_right(), symbol_table, containing_type_decl
        ));
    }
    else
    {
        get_right().emit_ir(containing_block, symbol_table, containing_type_decl);
    }

    const auto &variable_symbol = get_left().get_resolved();
    if (const auto &local_var = dynamic_cast<const semantic_analyzer::local_variable_symbol *>(&variable_symbol))
    {
        containing_block.add_instruction(std::make_unique<output::ir::store_in_local_var_instruction>(
            local_var->get_type()->to_instruction_type(),
            local_var->get_jvm_index()
        ));
    }
    else
    {
        throw std::runtime_error("Variable type not yet supported");
    }
}
