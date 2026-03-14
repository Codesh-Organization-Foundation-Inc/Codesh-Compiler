#pragma once

#include "defenition/primitive_type.h"

#include <memory>
#include <optional>

namespace codesh::ast::var_reference
{
class value_ast_node;
}
namespace codesh::ast::type
{
class type_ast_node;
}

namespace codesh::semantic_analyzer::util
{

struct widen_result
{
    bool are_types_compatible;
    std::unique_ptr<ast::var_reference::value_ast_node> value;
};


/**
 * @returns True if @p from can be widened to @p to per the JVM specs, false otherwise.
 */
[[nodiscard]] bool can_widen_to(definition::primitive_type from, definition::primitive_type to);

/**
 * Requires both to be primitive_type_ast_nodes; Returns false otherwise.
 *
 * @returns True if @p from can be widened to @p to per the JVM specs, false otherwise.
 */
[[nodiscard]] bool can_widen_to(const ast::type::type_ast_node &from, const ast::type::type_ast_node &to);

/**
 * @return If the two primitive types are compatible via widening, returns the wider one.
 * Returns @c std::nullopt if incompatible or not both primitive.
 */
[[nodiscard]] std::optional<definition::primitive_type> get_widened_primitive_type(const ast::type::type_ast_node &from,
        const ast::type::type_ast_node &to);

/**
 * @returns The @p value_node wrapped in a @c widening_cast_ast_node, @p value_node itself if unnecessary.
 */
[[nodiscard]] widen_result widen_value(std::unique_ptr<ast::var_reference::value_ast_node> value_node,
    const ast::type::type_ast_node &expected_type);

}
