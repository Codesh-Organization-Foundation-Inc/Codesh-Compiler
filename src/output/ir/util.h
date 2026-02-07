#pragma once

#include "code_block.h"
#include "instruction/impl/typed_instruction.h"

#include <optional>

namespace codesh::ast::var_reference
{
class value_ast_node;
}
namespace codesh::output::ir::util
{
void emit_assignment_by_value_optimized(code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl,
        const ast::var_reference::value_ast_node &value_node, instruction_type type, operator_type op_type,
        int target_lvt_index, std::optional<int> constant_rhs_cpi);
}
