#pragma once

#include "defenition/primitive_type.h"

#include <optional>

namespace codesh::ast::type
{
class type_ast_node;
}

namespace codesh::semantic_analyzer::util
{

/**
 * @returns True if @p from can be widened to @p to per the JVM specs, false otherwise.
 */
bool can_widen_to(definition::primitive_type from, definition::primitive_type to);

/**
 * Requires both to be primitive_type_ast_nodes; Returns false otherwise.
 *
 * @returns True if @p from can be widened to @p to per the JVM specs, false otherwise.
 */
bool can_widen_to(const ast::type::type_ast_node &from, const ast::type::type_ast_node &to);

/**
 * @return If the two primitive types are compatible via widening, returns the wider one.
 * Returns @c std::nullopt if incompatible or not both primitive.
 */
std::optional<definition::primitive_type> get_widened_primitive_type(const ast::type::type_ast_node &from,
        const ast::type::type_ast_node &to);

}
