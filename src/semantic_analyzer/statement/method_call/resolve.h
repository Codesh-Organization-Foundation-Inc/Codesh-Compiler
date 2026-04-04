#pragma once

#include "semantic_analyzer/statement/resolve.h"

#include <optional>

namespace codesh::ast::method::operation
{
class method_call_ast_node;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}

namespace codesh::semantic_analyzer::statement::method_call
{
bool resolve(const semantic_context &context, ast::method::operation::method_call_ast_node &method_call,
             const method_scope_info &method_info);
}
