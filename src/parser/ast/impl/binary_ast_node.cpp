#include "binary_ast_node.h"

#include "blasphemy/blasphemy_collector.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type/widening_cast_ast_node.h"
#include "semantic_analyzer/util.h"
#include "semantic_analyzer/util/widen_util.h"

codesh::ast::impl::binary_ast_node::binary_ast_node(const blasphemy::code_position code_position,
                                                    std::unique_ptr<value_ast_node> left, std::unique_ptr<value_ast_node> right) :
    operator_ast_node(code_position)
{
    children[0] = std::move(left);
    children[1] = std::move(right);
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::binary_ast_node::get_left() const
{
    return *children[0];
}

codesh::ast::var_reference::value_ast_node &codesh::ast::impl::binary_ast_node::get_right() const
{
    return *children[1];
}

bool codesh::ast::impl::binary_ast_node::is_value_valid() const
{
    return semantic_analyzer::util::do_types_match(*get_left().get_type(), *get_right().get_type());
}

bool codesh::ast::impl::binary_ast_node::apply_widening_conversions()
{
    const auto *left_type = get_left().get_type();
    const auto *right_type = get_right().get_type();
    if (!left_type || !right_type)
        return false;

    if (semantic_analyzer::util::do_types_match(*left_type, *right_type))
        return true;

    const auto *left_prim = dynamic_cast<const type::primitive_type_ast_node *>(left_type);
    const auto *right_prim = dynamic_cast<const type::primitive_type_ast_node *>(right_type);

    if (!left_prim || !right_prim)
        return false;

    if (semantic_analyzer::util::can_widen_to(left_prim->get_type(), right_prim->get_type()))
    {
        wrap_child_in_widening_cast(0, right_prim->get_type());
        return true;
    }
    if (semantic_analyzer::util::can_widen_to(right_prim->get_type(), left_prim->get_type()))
    {
        wrap_child_in_widening_cast(1, left_prim->get_type());
        return true;
    }

    return false;
}

void codesh::ast::impl::binary_ast_node::wrap_child_in_widening_cast(const std::size_t index,
        const definition::primitive_type target)
{
    const auto pos = children[index]->get_code_position();

    children[index] = std::make_unique<type::widening_cast_ast_node>(
        pos,
        std::move(children[index]),
        std::make_unique<type::primitive_type_ast_node>(pos, target)
    );
}

codesh::ast::type::type_ast_node *codesh::ast::impl::binary_ast_node::get_type() const
{
    return get_left().get_type();
}
