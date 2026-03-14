#include "widen_util.h"

#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type/widening_cast_ast_node.h"
#include "parser/ast/var_reference/value_ast_node.h"
#include "semantic_analyzer/util.h"

#include <cassert>
#include <set>
#include <unordered_map>

using namespace codesh::definition;

static const std::unordered_map<primitive_type, std::set<primitive_type>> WIDENING_MAP = {
    {
        primitive_type::BYTE,
        {
            primitive_type::SHORT,
            primitive_type::INTEGER,
            primitive_type::LONG,
            primitive_type::FLOAT,
            primitive_type::DOUBLE
        }
    },
    {
        primitive_type::SHORT,
        {
            primitive_type::INTEGER,
            primitive_type::LONG,
            primitive_type::FLOAT,
            primitive_type::DOUBLE
        }
    },
    {
        primitive_type::CHAR,
        {
            primitive_type::INTEGER,
            primitive_type::LONG,
            primitive_type::FLOAT,
            primitive_type::DOUBLE
        }
    },
    {
        primitive_type::INTEGER,
        {
            primitive_type::LONG,
            primitive_type::FLOAT,
            primitive_type::DOUBLE
        }
    },
    {
        primitive_type::LONG,
        {
            primitive_type::FLOAT,
            primitive_type::DOUBLE
        }
    },
    {
        primitive_type::FLOAT,
        {
            primitive_type::DOUBLE
        }
    },
};

bool codesh::semantic_analyzer::util::can_widen_to(const primitive_type from, const primitive_type to)
{
    const auto it = WIDENING_MAP.find(from);
    if (it == WIDENING_MAP.end())
        return false;

    return it->second.contains(to);
}

bool codesh::semantic_analyzer::util::can_widen_to(const ast::type::type_ast_node &from,
                                                   const ast::type::type_ast_node &to)
{
    const auto *from_prim = dynamic_cast<const ast::type::primitive_type_ast_node *>(&from);
    const auto *to_prim = dynamic_cast<const ast::type::primitive_type_ast_node *>(&to);
    if (!from_prim || !to_prim)
        return false;

    return can_widen_to(from_prim->get_type(), to_prim->get_type());
}

std::optional<primitive_type> codesh::semantic_analyzer::util::get_widened_primitive_type(
        const ast::type::type_ast_node &from,
        const ast::type::type_ast_node &to)
{
    const auto *from_prim = dynamic_cast<const ast::type::primitive_type_ast_node *>(&from);
    const auto *to_prim = dynamic_cast<const ast::type::primitive_type_ast_node *>(&to);
    if (!from_prim || !to_prim)
        return std::nullopt;

    const auto from_type = from_prim->get_type();
    const auto to_type = to_prim->get_type();

    if (from_type == to_type)
        return from_type;
    if (can_widen_to(from_type, to_type))
        return to_type;
    if (can_widen_to(to_type, from_type))
        return from_type;

    return std::nullopt;
}

codesh::semantic_analyzer::util::widen_result codesh::semantic_analyzer::util::widen_value(
        std::unique_ptr<ast::var_reference::value_ast_node> value_node,
        const ast::type::type_ast_node &expected_type)
{
    assert(value_node->get_type() != nullptr && "Value must have a type");
    const auto &type = *value_node->get_type();

    if (do_types_match(type, expected_type))
        return {true, (std::move(value_node))};

    if (can_widen_to(type, expected_type))
    {
        const auto &expected_prim_type = dynamic_cast<const ast::type::primitive_type_ast_node &>(expected_type);
        const auto code_pos = value_node->get_code_position();

        return {
            true,
            std::make_unique<ast::type::widening_cast_ast_node>(
                code_pos,
                std::move(value_node),
                std::make_unique<ast::type::primitive_type_ast_node>(
                    code_pos,
                    expected_prim_type.get_type()
                )
            )
        };
    }

    return {false, (std::move(value_node))};
}
