#pragma once

#include "semantic_analyzer/statement/resolve.h"

namespace codesh::ast::var_reference
{
class variable_reference_ast_node;
}
namespace codesh::ast::method::operation
{
class method_call_ast_node;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}

namespace codesh::semantic_analyzer::statement::variable_reference
{
/**
 * @returns Whether the operation succeed
 */
bool resolve(const semantic_context &context, ast::var_reference::variable_reference_ast_node &var_ref_node,
             const std::optional<method_scope_info> &method_info);
}
