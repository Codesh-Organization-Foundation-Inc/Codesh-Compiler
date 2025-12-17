#pragma once

#include "../parser/ast/compilation_unit_ast_node.h"

#include <functional>

namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::semantic_analyzer
{
extern const std::string DEFAULT_SUPER_CLASS_NAME;

/**
 * This runs before the symbol table is constructed.
 */
void prepare(const ast::compilation_unit_ast_node &ast_root);
/**
 * This runs after the symbol table is constructed.
 */
void analyze(const ast::compilation_unit_ast_node &ast_root);
}
