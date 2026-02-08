#include "util.h"

#include "instruction/assignment_from_code_block_instruction.h"
#include "instruction/increment_by_constant_instruction.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"

static constexpr short MIN_INT_CONSTANT = std::numeric_limits<int16_t>::min();
static constexpr short MAX_INT_CONSTANT = std::numeric_limits<int16_t>::max();


std::optional<int> codesh::output::ir::util::goc_big_value(const ast::var_reference::value_ast_node &value_node,
        jvm_target::constant_pool &constant_pool)
{
    //TODO: Handle non-int
    if (const auto evaluable = dynamic_cast<const ast::var_reference::evaluable_ast_node<int> *>(&value_node))
    {
        const auto skip_constant = evaluable->get_value();

        if (skip_constant < MIN_INT_CONSTANT || skip_constant > MAX_INT_CONSTANT)
        {
            return constant_pool.goc_integer_info(skip_constant);
        }
    }

    return std::nullopt;
}

void codesh::output::ir::util::emit_assignment_by_value_optimized(code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const ast::var_reference::value_ast_node &value_node, instruction_type type, operator_type op_type,
        int target_lvt_index, std::optional<int> constant_rhs_cpi)
{
    //TODO: Handle non-int
    if (const auto evaluable = dynamic_cast<const ast::var_reference::evaluable_ast_node<int> *>(&value_node))
    {
        const auto value = evaluable->get_value();

        containing_block.add_instruction(std::make_unique<increment_by_constant_instruction>(
            type, // Always int rn
            target_lvt_index,
            value,
            constant_rhs_cpi
        ));
    }
    else
    {
        code_block skip_block;
        value_node.emit_ir(skip_block, symbol_table, containing_type_decl);

        containing_block.add_instruction(std::make_unique<assignment_from_code_block_instruction>(
            type,
            op_type,
            target_lvt_index,
            std::move(skip_block)
        ));
    }
}
