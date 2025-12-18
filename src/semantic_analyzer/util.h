#pragma once

#include "symbol_table/symbol.h"

#include <string>
#include <vector>

namespace codesh::definition
{
class fully_qualified_class_name;
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
//FIXME: Do not return optional: return an std::pair of whether succeed and the provided name as-is.
//FIXME: Use FQCNs, NOT strings.

/**
 * @returns the Fully Qualified Class Name of the given type name (which isn't a descriptor).
 * If unresolved, returns {@link std::nullopt}
 */
[[nodiscard]] std::optional<std::string> resolve_custom_type(
        const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        //FIXME: This should get an FQCN
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
