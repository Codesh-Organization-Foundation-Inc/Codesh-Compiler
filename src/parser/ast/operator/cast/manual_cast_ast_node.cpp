#include "manual_cast_ast_node.h"

#include "output/ir/code_block.h"
#include "output/ir/instruction/checkcast_instruction.h"
#include "output/ir/instruction/narrowing_cast_instruction.h"
#include "output/ir/instruction/widening_cast_instruction.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"
#include "semantic_analyzer/util/poly_util.h"
#include "semantic_analyzer/util/widen_util.h"

codesh::ast::op::assignment::manual_cast_ast_node::manual_cast_ast_node(
        const lexer::code_position code_position, std::unique_ptr<value_ast_node> value,
        std::unique_ptr<type::type_ast_node> target_type) :
    cast_ast_node(code_position, std::move(value)),
    target_type(std::move(target_type))
{
}

codesh::ast::type::type_ast_node *codesh::ast::op::assignment::manual_cast_ast_node::get_type() const
{
    return target_type.get();
}

void codesh::ast::op::assignment::manual_cast_ast_node::emit_constants(
        const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool)
{
    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(&get_value()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }

    if (const auto custom_target = dynamic_cast<type::custom_type_ast_node *>(target_type.get()))
    {
        constant_pool.goc_class_info(
            constant_pool.goc_utf8_info(custom_target->get_resolved_name().join())
        );
    }
}

void codesh::ast::op::assignment::manual_cast_ast_node::emit_ir(
        output::ir::code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const type_decl::type_declaration_ast_node &containing_type_decl) const
{
    get_value().emit_ir(containing_block, symbol_table, containing_type_decl);

    if (try_emit_primitive_cast(containing_block, *get_value().get_type(), *target_type))
        return;

    // Reference cast - no-op for upcasts, checkcast for downcasts
    if (semantic_analyzer::util::can_poly_cast_to(*get_value().get_type(), *target_type))
        return;

    const auto *custom_target = dynamic_cast<type::custom_type_ast_node *>(target_type.get());
    const auto &cp = containing_type_decl.get_constant_pool();
    const int class_cpi = cp.get_class_index(
        cp.get_utf8_index(custom_target->get_resolved_name().join())
    );

    containing_block.add_instruction(
        std::make_unique<output::ir::checkcast_instruction>(class_cpi)
    );
}

bool codesh::ast::op::assignment::manual_cast_ast_node::try_emit_primitive_cast(
        output::ir::code_block &containing_block,
        const type::type_ast_node &source_type, const type::type_ast_node &target_type)
{
    const auto *source_prim = dynamic_cast<const type::primitive_type_ast_node *>(&source_type);
    const auto *target_prim = dynamic_cast<const type::primitive_type_ast_node *>(&target_type);
    if (!source_prim || !target_prim)
        return false;

    const auto from = source_prim->get_type();
    const auto to = target_prim->get_type();
    if (from == to)
        return true;

    // Sub-int types (char, boolean etc.)
    // Same size, no casting needed
    if (source_prim->to_instruction_type() == target_prim->to_instruction_type())
        return true;

    if (semantic_analyzer::util::can_widen_to(from, to))
    {
        containing_block.add_instruction(
            std::make_unique<output::ir::widening_cast_instruction>(
                source_prim->to_instruction_type(),
                target_prim->to_instruction_type()
            )
        );
    }
    else
    {
        containing_block.add_instruction(
            std::make_unique<output::ir::narrowing_cast_instruction>(from, to)
        );
    }

    return true;
}
