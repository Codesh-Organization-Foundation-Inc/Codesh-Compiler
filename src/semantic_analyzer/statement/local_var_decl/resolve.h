#pragma once

namespace codesh::ast
{
class local_variable_declaration_ast_node;
}
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

namespace codesh::semantic_analyzer::statement::local_var_decl
{
void resolve(const semantic_context &context, ast::local_variable_declaration_ast_node &var_decl_node,
        const method_symbol &containing_method);
}
