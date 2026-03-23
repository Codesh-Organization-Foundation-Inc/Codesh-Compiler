#pragma once

namespace codesh::ast::op::assignment
{
class cast_ast_node;
}
namespace codesh::semantic_analyzer
{
class method_scope_symbol;
struct semantic_context;
class method_symbol;
}

namespace codesh::semantic_analyzer::statement::cast
{
/**
 * @returns Whether the operation succeeded
 */
bool resolve(const semantic_context &context, const ast::op::assignment::cast_ast_node &cast,
        const method_symbol &containing_method, const method_scope_symbol &scope);
}
