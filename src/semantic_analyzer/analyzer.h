#pragma once

#include "parser/ast/compilation_unit_ast_node.h"

namespace codesh::semantic_analyzer
{
class symbol_table;
}


namespace codesh::semantic_analyzer
{
extern const definition::fully_qualified_name DEFAULT_SUPER_CLASS_NAME;

/**
 * This runs before the symbol table is constructed.
 */
void prepare(const ast::compilation_unit_ast_node &ast_root);
/**
 * This runs after the symbol table is constructed.
 */
void collect_symbols(const ast::compilation_unit_ast_node &ast_root, const symbol_table &table);
void post_collect(const ast::compilation_unit_ast_node &ast_root, const symbol_table &table);
void analyze(const ast::compilation_unit_ast_node &ast_root, const symbol_table &table);
}
