#pragma once

namespace codesh::ast::method::operation
{
class method_call_ast_node;
}
namespace codesh::semantic_analyzer
{
class method_scope_symbol;
struct semantic_context;
}
namespace codesh::semantic_analyzer
{
class method_symbol;
}

namespace codesh::semantic_analyzer::statement::method_call
{
bool resolve(const semantic_context &context, ast::method::operation::method_call_ast_node &method_call,
        const method_symbol &containing_method, const method_scope_symbol &scope);
}
