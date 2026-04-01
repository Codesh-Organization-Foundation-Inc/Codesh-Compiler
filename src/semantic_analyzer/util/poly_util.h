#pragma once

namespace codesh::semantic_analyzer
{
struct semantic_context;
class type_symbol;
}
namespace codesh::ast::type
{
class type_ast_node;
}

namespace codesh::semantic_analyzer::util
{

/**
 * @returns @c true if `subtype` is the same as or a subtype of `supertype`
 */
[[nodiscard]] bool is_subtype_of(const semantic_context &context, const type_symbol &subtype,
        const type_symbol &supertype);

/**
 * @returns @c true if `from` is a reference type that is assignable to `to`
 * via the subtype relationship (implicit upcast).
 * Returns false for primitives.
 */
[[nodiscard]] bool can_poly_cast_to(const semantic_context &context, const ast::type::type_ast_node &from,
        const ast::type::type_ast_node &to);

}
