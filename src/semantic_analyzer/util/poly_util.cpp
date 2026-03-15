#include "poly_util.h"

#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

bool codesh::semantic_analyzer::util::is_subtype_of(const type_symbol &subtype, const type_symbol &supertype)
{
    if (&subtype == &supertype)
        return true;

    if (subtype.has_super_type())
    {
        const auto &super_node = subtype.get_super_type();
        if (super_node.is_resolved())
        {
            if (is_subtype_of(super_node.get_resolved(), supertype))
                return true;
        }
    }

    for (const auto &interface : subtype.get_interfaces())
    {
        if (interface->is_resolved())
        {
            if (is_subtype_of(interface->get_resolved(), supertype))
                return true;
        }
    }

    return false;
}

bool codesh::semantic_analyzer::util::can_poly_cast_to(const ast::type::type_ast_node &from,
                                                       const ast::type::type_ast_node &to)
{
    const auto *from_custom = dynamic_cast<const ast::type::custom_type_ast_node *>(&from);
    const auto *to_custom = dynamic_cast<const ast::type::custom_type_ast_node *>(&to);

    if (!from_custom || !to_custom)
        return false;

    if (!from_custom->is_resolved() || !to_custom->is_resolved())
        return false;

    return is_subtype_of(from_custom->get_resolved(), to_custom->get_resolved());
}
