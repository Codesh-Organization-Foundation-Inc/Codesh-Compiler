#pragma once

#include "semantic_analyzer/symbol_table/symbol.h"

#include <string>

namespace codesh::semantic_analyzer
{
struct semantic_context;
}
namespace codesh::definition
{
class fully_qualified_name;
}
namespace codesh::ast
{
namespace type
{
class custom_type_ast_node;
}
class compilation_unit_ast_node;
}

namespace codesh::semantic_analyzer::util
{
/**
 * @returns the Fully Qualified Class Name of the given type name.
 * If unresolved, returns `false` and the originally passed name.
 * Otherwise, returns `true` and the resolved name.
 */
[[nodiscard]] std::optional<std::reference_wrapper<type_symbol>> resolve_custom_type(const semantic_context &context,
        const ast::type::custom_type_ast_node &custom_type_node);

/**
 * @return Whether the type was successfully resolved
 * @param related_type_node A 2nd node that will get resolved with the exact same result, if found.
 */
bool resolve_custom_type_node(const semantic_context &context, ast::type::custom_type_ast_node &custom_type_node,
        std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node = std::nullopt);

/**
 * Resolves the provided type only if it is a custom type. Otherwise, marks it as resolved (since only a custom type
 * is relevant to be resolved).
 *
 * For instance, a boolean is simply a primitive boolean that doesn't link anywhere.
 *
 * @return Whether the type was successfully resolved
 * @param related_type_node A 2nd node that will get resolved with the exact same result, if found.
 */
bool resolve_type_node(const semantic_context &context, ast::type::type_ast_node &type_node,
        std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node = std::nullopt);

bool are_types_compatible(const ast::type::type_ast_node &from, const ast::type::type_ast_node &to);

/**
 * Either creates or returns the requested method overloads symbol.
 */
[[nodiscard]] method_overloads_symbol &get_method_overloads_symbol(const std::string &name,
        type_symbol &containing_type);
}
