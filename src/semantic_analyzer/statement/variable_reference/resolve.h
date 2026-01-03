#pragma once

class variable_reference_ast_node;
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

namespace codesh::semantic_analyzer::statement::variable_reference
{
/**
 * @returns Whether the operation succeed
 */
bool resolve(const semantic_context &context, variable_reference_ast_node &var_ref_node,
             const method_scope_symbol &scope);
}
