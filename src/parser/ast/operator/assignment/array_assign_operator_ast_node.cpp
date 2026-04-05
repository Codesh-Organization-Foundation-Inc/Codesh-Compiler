#include "array_assign_operator_ast_node.h"

#include "lexer/trie/keywords.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/array_store_instruction.h"
#include "semantic_analyzer/util/poly_util.h"

codesh::ast::op::assignment::array_assign_operator_ast_node::array_assign_operator_ast_node(
        const lexer::code_position code_position, std::unique_ptr<op::array_access_ast_node> left,
        std::unique_ptr<value_ast_node> right) :
    binary_ast_node(code_position, std::move(left), std::move(right))
{
}

std::string codesh::ast::op::assignment::array_assign_operator_ast_node::to_pretty_string() const
{
    return lexer::trie::token_to_string(token_group::KEYWORD_REPLACE);
}

codesh::ast::type::type_ast_node *codesh::ast::op::assignment::array_assign_operator_ast_node::get_type() const
{
    return get_left().get_type();
}

bool codesh::ast::op::assignment::array_assign_operator_ast_node::is_value_valid(
        const semantic_analyzer::semantic_context &context) const
{
    if (binary_ast_node::is_value_valid(context))
        return true;

    const auto *lhs_type = get_left().get_type();
    const auto *rhs_type = get_right().get_type();
    if (!lhs_type || !rhs_type)
        return false;

    return semantic_analyzer::util::can_poly_cast_to(context, *rhs_type, *lhs_type);
}

void codesh::ast::op::assignment::array_assign_operator_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto &arr_access = static_cast<const array_access_ast_node &>(get_left()); // NOLINT(*-pro-type-static-cast-downcast)

    arr_access.get_array().emit_ir(containing_block, symbol_table, containing_type_decl);
    arr_access.get_index().emit_ir(containing_block, symbol_table, containing_type_decl);

    containing_block.set_is_consuming(true);
    get_right().emit_ir(containing_block, symbol_table, containing_type_decl);
    containing_block.set_is_consuming(false);

    containing_block.add_instruction(
        std::make_unique<output::ir::array_store_instruction>(arr_access.get_type()->to_instruction_type())
    );
}
