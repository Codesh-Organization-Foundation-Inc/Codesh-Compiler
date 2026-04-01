#pragma once

#include "defenition/primitive_type.h"

#include <memory>
#include <optional>

namespace codesh::semantic_analyzer
{
struct semantic_context;
}
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
 * @returns @c true if @p from can be widened to @p to per the JVM specs, false otherwise.
 */
[[nodiscard]] bool can_widen_to(definition::primitive_type from, definition::primitive_type to);

/**
 * @returns @c true if @p from can be widened to @p to per the JVM specs, false otherwise.
 * Returns @c false for non-primitives.
 */
[[nodiscard]] bool can_widen_to(const ast::type::type_ast_node &from, const ast::type::type_ast_node &to);

/**
 * @return If the two primitive types are compatible via widening, returns the wider one.
 * Returns @c std::nullopt if incompatible or not both primitive.
 */
[[nodiscard]] std::optional<definition::primitive_type> get_widened_primitive_type(const ast::type::type_ast_node &from,
        const ast::type::type_ast_node &to);

/**
 * Wraps @p value_node in a @c widening_cast_ast_node targeting @p target_type only if it's REALLY needed.
 */
[[nodiscard]] widen_result make_widening_cast_maybe(const semantic_context &context,
        std::unique_ptr<ast::var_reference::value_ast_node> value_node,
        const ast::type::type_ast_node &expected_type);

/**
 * Wraps @p value_node in a @c widening_cast_ast_node targeting @p target_type
 */
[[nodiscard]] std::unique_ptr<ast::var_reference::value_ast_node> make_widening_cast(
        std::unique_ptr<ast::var_reference::value_ast_node> value_node,
        const ast::type::type_ast_node &target_type);

}
