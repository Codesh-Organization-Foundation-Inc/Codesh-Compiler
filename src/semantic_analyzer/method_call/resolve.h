#pragma once

namespace codesh::ast::method::operation
{
class method_call_ast_node;
}
namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::semantic_analyzer
{
class method_symbol;
}

namespace codesh::semantic_analyzer::method_call
{
void resolve(const semantic_context &context, ast::method::operation::method_call_ast_node &method_call,
        const method_symbol &containing_method);
}
