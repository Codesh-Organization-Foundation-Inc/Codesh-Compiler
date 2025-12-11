#pragma once

#include "symbol_table/symbol.h"

#include <string>
#include <vector>

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
 * @returns the Fully Qualified Class Name of the given type name (which isn't a descriptor).
 * If unresolved, returns {@link std::nullopt}
 */
std::optional<std::string> resolve_custom_type(
        const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        const std::string &name);

/**
 * @return Whether the type was successfully resolved
 */
bool resolve_custom_type_node(const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        ast::type::custom_type_ast_node &symbol_type_node);

/**
 * @return Whether the type was successfully resolved
 */
bool resolve_custom_type_node(const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        ast::type::custom_type_ast_node &custom_type_node,
        ast::type::type_ast_node &symbol_type_node);

/**
 * Either creates or returns the requested method overloads symbol.
 */
[[nodiscard]] method_overloads_symbol &get_method_overloads_symbol(const std::string &name,
        type_symbol &containing_type);
}
