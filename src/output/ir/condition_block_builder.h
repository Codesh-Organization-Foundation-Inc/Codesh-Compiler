#pragma once

#include "instruction/if_instruction.h"
#include "output/ir/code_block.h"
#include "output/ir/instruction/impl/opcode.h"

namespace codesh::ast::var_reference
{
class value_ast_node;
}
namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}

namespace codesh::output::ir
{

code_block build_condition_block(const ast::var_reference::value_ast_node &condition, size_t if_block_size,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl, if_type if_type);

/**
 * Builds a condition block that loads 1 to the stack if the condition is met, or 0 otherwise
 */
code_block build_boolean_value_block(const ast::var_reference::value_ast_node &condition,
        const semantic_analyzer::symbol_table &symbol_table,
        const ast::type_decl::type_declaration_ast_node &containing_type_decl);

}
