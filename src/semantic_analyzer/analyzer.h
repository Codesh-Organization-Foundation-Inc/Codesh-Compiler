#pragma once

#include "parser/ast/compilation_unit_ast_node.h"
#include "semantic_context.h"

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
 * Builds the semantic context for a compilation unit.
 *
 * The context can then be reused across all analysis passes for the same unit.
 */
[[nodiscard]] semantic_context make_context(const ast::compilation_unit_ast_node &ast_root,
        const symbol_table &table);

/**
 * These run after the symbol table is constructed.
 */
void collect_symbols(const semantic_context &context);
void post_collect(const semantic_context &context);
void analyze(const semantic_context &context);
}
