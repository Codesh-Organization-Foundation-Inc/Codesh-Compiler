#include "poly_util.h"

#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/util.h"

bool codesh::semantic_analyzer::util::is_subtype_of(const semantic_context &context, const type_symbol &subtype,
    const type_symbol &supertype)
{
    if (subtype.get_full_name() == supertype.get_full_name())
        return true;

    if (subtype.has_super_type())
    {
        if (const auto &super =
            resolve_custom_type_node(context, subtype.get_super_type()))
        {
            if (is_subtype_of(context, super->get(), supertype))
                return true;
        }

    }

    for (const auto &interface_node : subtype.get_interfaces())
    {
        if (const auto &interface =
            resolve_custom_type_node(context, *interface_node))
        {
            if (is_subtype_of(context, interface->get(), supertype))
                return true;
        }
    }

    return false;
}

bool codesh::semantic_analyzer::util::can_poly_cast_to(const semantic_context &context,
        const ast::type::type_ast_node &from, const ast::type::type_ast_node &to)
{
    const auto *from_custom = dynamic_cast<const ast::type::custom_type_ast_node *>(&from);
    const auto *to_custom = dynamic_cast<const ast::type::custom_type_ast_node *>(&to);

    if (!from_custom || !to_custom)
        return false;

    if (!from_custom->is_resolved() || !to_custom->is_resolved())
        return false;

    return is_subtype_of(context, from_custom->get_resolved(), to_custom->get_resolved());
}
