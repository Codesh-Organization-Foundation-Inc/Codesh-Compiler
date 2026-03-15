#pragma once

#include "semantic_analyzer/symbol_table/symbol.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

//TODO: Move file to ./util
namespace codesh::semantic_analyzer
{
struct semantic_context;
class symbol_table;
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
class type_ast_node;
}
namespace type_decl
{
class type_declaration_ast_node;
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
 * @return The resolved type symbol if it was successfully resolved - @c std::nullopt otherwise
 * @param context
 * @param custom_type_node
 * @param related_type_node A 2nd node that will get resolved with the exact same result, if found.
 */
std::optional<std::reference_wrapper<type_symbol>> resolve_custom_type_node(const semantic_context &context,
        ast::type::custom_type_ast_node &custom_type_node,
        std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node = std::nullopt);

/**
 * @return The resolved type symbol if the provided @p type_node was a @c custom_type_node and was successfully
 * resolved - @c std::nullopt otherwise
 * @param context
 * @param type_node
 * @param related_type_node A 2nd node that will get resolved with the exact same result, if found.
 */
std::optional<std::reference_wrapper<type_symbol>> resolve_custom_type_node(const semantic_context &context,
        ast::type::type_ast_node &type_node,
        std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node = std::nullopt);

/**
 * Resolves the provided type only if it is a custom type. Otherwise, marks it as resolved (since only a custom type
 * is relevant to be resolved).
 *
 * For instance, a boolean is simply a primitive boolean that doesn't link anywhere.
 *
 * @return Whether the type was successfully resolved
 * @param context
 * @param type_node
 * @param related_type_node A 2nd node that will get resolved with the exact same result, if found.
 */
bool resolve_type_node(const semantic_context &context, ast::type::type_ast_node &type_node,
        std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node = std::nullopt);

bool do_types_match(const ast::type::type_ast_node &from, const ast::type::type_ast_node &to);

/**
 * Either creates or returns the requested method overloads symbol.
 */
[[nodiscard]] method_overloads_symbol &get_method_overloads_symbol(const std::string &name,
        type_symbol &containing_type);

/**
 * Finds or creates the nested country_symbol hierarchy for a '/'-separated package name
 * within the symbol table, creating missing intermediate countries as needed.
 */
[[nodiscard]] country_symbol &find_or_create_country(const symbol_table &table,
        const std::string &package_name);

/**
 * Creates a @c type_symbol in the given @p country and adds it to its scope.
 * @returns The symbol and whether it did not exist before (operation succeed).
 */
[[nodiscard]] std::pair<std::reference_wrapper<type_symbol>, bool> add_type_symbol(country_symbol &country,
        const std::string &name, std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::unique_ptr<ast::type::custom_type_ast_node> super_type = nullptr,
        std::vector<std::unique_ptr<ast::type::custom_type_ast_node>> interfaces = {},
        ast::type_decl::type_declaration_ast_node *decl = nullptr);

/**
 * Creates a field_symbol in the given type and adds it to its scope.
 */
field_symbol &add_field_symbol(type_symbol &type_sym, const std::string &name,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::unique_ptr<ast::type::type_ast_node> type);
}
