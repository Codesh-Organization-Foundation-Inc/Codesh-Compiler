#include "assignment_operator_ast_node.h"

#include "output/ir/instruction/assignment_from_code_block_instruction.h"
#include "output/ir/instruction/store_in_local_var_instruction.h"
#include "output/ir/util.h"
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

void codesh::ast::op::assignment::assignment_operator_ast_node::emit_constants(
    const compilation_unit_ast_node &root_node, output::jvm_target::constant_pool &constant_pool)
{
    binary_ast_node::emit_constants(root_node, constant_pool);
    rhs_cpi = output::ir::util::goc_big_int_value(get_right(), constant_pool, get_operator_type());
}

void codesh::ast::op::assignment::assignment_operator_ast_node::emit_ir(
        output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    const auto &var_ref = get_left();
    const auto *local_var = dynamic_cast<const semantic_analyzer::local_variable_symbol *>(&var_ref.get_resolved());
    if (!local_var)
        throw std::runtime_error("Unsupported assignment target");

    const auto type = local_var->get_type()->to_instruction_type();
    const auto lvt_index = local_var->get_jvm_index();
    const auto op_type = get_operator_type();
    const auto &rhs = get_right();

    containing_block.set_is_consuming(true);
    if (op_type == output::ir::operator_type::ASSIGN)
    {
        // Emit the RHS directly
        rhs.emit_ir(containing_block, symbol_table, containing_type_decl);
        // Then immediately load it to the variable in question
        containing_block.add_instruction(std::make_unique<output::ir::store_in_local_var_instruction>(
            type, lvt_index
        ));
    }
    else
    {
        output::ir::util::emit_increment_by_value_optimized(
            containing_block, symbol_table, containing_type_decl,
            rhs, {type, op_type, lvt_index}, rhs_cpi
        );
    }
    containing_block.set_is_consuming(false);
}
