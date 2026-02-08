#pragma once

#include "code_block.h"
#include "instruction/impl/typed_instruction.h"

#include <optional>

namespace codesh::output::jvm_target
{
class constant_pool;
}
namespace codesh::ast::var_reference
{
class value_ast_node;
}


namespace codesh::output::ir::util
{
[[nodiscard]] std::optional<int> goc_big_int_value(const ast::var_reference::value_ast_node &value_node,
        jvm_target::constant_pool &constant_pool, operator_type applied_operator);

/**
 * If the incrementation is done by an RHS that is:
 *
 * 1. An evaluable
 * 2. An integer
 * 3. No longer than 2 bytes in size
 *
 * ...uses a single opcode (iinc).
 *
 * Otherwise, uses 4 opcodes.
 */
void emit_increment_by_value_optimized(code_block &containing_block,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const ast::var_reference::value_ast_node &rhs_value, instruction_type type, operator_type op_type,
        int target_lvt_index, std::optional<int> constant_int_rhs_cpi);
}
