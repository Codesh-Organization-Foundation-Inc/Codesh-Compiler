#pragma once

#include "semantic_analyzer/statement/resolve.h"

namespace codesh::ast::op::assignment
{
class cast_ast_node;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}

namespace codesh::semantic_analyzer::statement::cast
{
/**
 * @returns Whether the operation succeeded
 */
bool resolve(const semantic_context &context, ast::op::assignment::cast_ast_node &cast,
        const std::optional<method_scope_info> &method_info);
}
