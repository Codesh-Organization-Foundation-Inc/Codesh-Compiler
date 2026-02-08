#include "assignment_operator_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/store_in_local_var_instruction.h"
#include "semantic_analyzer/symbol_table/symbol.h"

codesh::ast::op::assignment::assignment_operator_ast_node::assignment_operator_ast_node(
        const blasphemy::code_position code_position, std::unique_ptr<variable_reference_ast_node> left,
        std::unique_ptr<value_ast_node> right) :
    binary_ast_node(code_position, std::move(left), std::move(right))
{
}

variable_reference_ast_node &codesh::ast::op::assignment::assignment_operator_ast_node::get_left() const
{
    return static_cast<variable_reference_ast_node &>(binary_ast_node::get_left()); // NOLINT(*-pro-type-static-cast-downcast)
}

codesh::ast::type::type_ast_node *codesh::ast::op::assignment::assignment_operator_ast_node::get_type() const
{
    // In assignment, the return type is determined by the left operand.
    return get_left().get_type();
}

void codesh::ast::op::assignment::assignment_operator_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto op = get_operator_type();

    // Compound assignments (+=, etc.) need the current variable value on the stack
    // before the right-hand side, so the operator can combine them.
    // Plain assignment (=) on the other hand only stores the right-hand side,
    // so loading the left value would be redundant.
    if (op.has_value())
    {
        get_left().emit_ir(containing_block, symbol_table, containing_type_decl);
    }

    get_right().emit_ir(containing_block, symbol_table, containing_type_decl);

    const auto &var_symbol = get_left().get_resolved();
    const auto instr_type = var_symbol.get_type()->to_instruction_type();

    // Load the LHS if compound assignment
    if (op.has_value())
    {
        containing_block.add_instruction(std::make_unique<output::ir::operator_instruction>(
            instr_type, *op
        ));
    }

    if (const auto &local_var = dynamic_cast<const semantic_analyzer::local_variable_symbol *>(&var_symbol))
    {
        containing_block.add_instruction(std::make_unique<output::ir::store_in_local_var_instruction>(
            instr_type, local_var->get_jvm_index()
        ));
    }
    else
    {
        throw std::runtime_error("Variable type not yet supported");
    }
}
