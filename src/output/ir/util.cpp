#include "util.h"

#include "instruction/assignment_from_code_block_instruction.h"
#include "instruction/increment_int_by_constant_instruction.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"

static constexpr short MIN_INT_CONSTANT = std::numeric_limits<int16_t>::min();
static constexpr short MAX_INT_CONSTANT = std::numeric_limits<int16_t>::max();

/**
 * @returns Whether the block was successfully emitted
 */
static bool emit_increment_by_evaluable(codesh::output::ir::code_block &containing_block,
        const codesh::ast::var_reference::evaluable_ast_node<int> &evaluable,
        const codesh::output::ir::lvt_operand &operand, std::optional<int> constant_int_rhs_cpi);

static void emit_increment_by_value(codesh::output::ir::code_block &containing_block,
        const codesh::semantic_analyzer::symbol_table &symbol_table,
        const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const codesh::ast::var_reference::value_ast_node &rhs_value,
        const codesh::output::ir::lvt_operand &operand);


std::optional<int> codesh::output::ir::util::goc_big_int_value(const ast::var_reference::value_ast_node &rhs_value,
        jvm_target::constant_pool &constant_pool, const operator_type applied_operator)
{
    if (const auto evaluable = dynamic_cast<const ast::var_reference::evaluable_ast_node<int> *>(&rhs_value))
    {
        const auto value = applied_operator == operator_type::SUB
            ? -evaluable->get_value()
            : evaluable->get_value();

        if (value < MIN_INT_CONSTANT || value > MAX_INT_CONSTANT)
        {
            return constant_pool.goc_integer_info(value);
        }
    }

    return std::nullopt;
}

void codesh::output::ir::util::emit_increment_by_value_optimized(code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const ast::var_reference::value_ast_node &rhs_value, const lvt_operand &operand,
        const std::optional<int> constant_int_rhs_cpi)
{
    // Prefer optimized version
    if (const auto evaluable = dynamic_cast<const ast::var_reference::evaluable_ast_node<int> *>(&rhs_value))
    {
        const auto did_emit = emit_increment_by_evaluable(
            containing_block,
            *evaluable,
            operand,
            constant_int_rhs_cpi
        );

        if (did_emit)
            return;
    }

    emit_increment_by_value(
        containing_block,
        symbol_table,
        containing_type_decl,
        rhs_value,
        operand
    );
}


static bool emit_increment_by_evaluable(codesh::output::ir::code_block &containing_block,
        const codesh::ast::var_reference::evaluable_ast_node<int> &evaluable,
        const codesh::output::ir::lvt_operand &operand, std::optional<int> constant_int_rhs_cpi)
{
    if (operand.op_type != codesh::output::ir::operator_type::ADD && operand.op_type != codesh::output::ir::operator_type::SUB)
        return false;

    const auto value = operand.op_type == codesh::output::ir::operator_type::SUB
        ? -evaluable.get_value()
        : evaluable.get_value();

    containing_block.add_instruction(std::make_unique<codesh::output::ir::increment_int_by_constant_instruction>(
        operand.lvt_index,
        value,
        constant_int_rhs_cpi
    ));

    return true;
}

static void emit_increment_by_value(codesh::output::ir::code_block &containing_block,
        const codesh::semantic_analyzer::symbol_table &symbol_table,
        const codesh::ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const codesh::ast::var_reference::value_ast_node &rhs_value,
        const codesh::output::ir::lvt_operand &operand)
{
    codesh::output::ir::code_block skip_block;
    rhs_value.emit_ir(skip_block, symbol_table, containing_type_decl);

    containing_block.add_instruction(std::make_unique<codesh::output::ir::assignment_from_code_block_instruction>(
        operand.type,
        operand.op_type,
        operand.lvt_index,
        std::move(skip_block)
    ));
}
