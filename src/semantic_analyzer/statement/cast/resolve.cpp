#include "resolve.h"

#include "semantic_analyzer/statement/resolve.h"
#include "semantic_analyzer/statement/variable_reference/resolve.h"

#include "blasphemy/details.h"
#include "fmt/xchar.h"
#include "parser/ast/operator/cast/cast_ast_node.h"
#include "parser/ast/operator/cast/manual_cast_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type_declaration/interface_declaration_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/util.h"
#include "semantic_analyzer/util/poly_util.h"

using namespace codesh::semantic_analyzer;

static bool is_valid_reference_cast(const semantic_context &context,
        codesh::ast::op::assignment::cast_ast_node &cast);
static bool is_primitive(const codesh::ast::type::type_ast_node *type);
static bool is_interface(const type_symbol &sym);
static bool is_custom_type(const codesh::ast::type::type_ast_node *type);

bool statement::cast::resolve(const semantic_context &context, ast::op::assignment::cast_ast_node &cast,
        const std::optional<method_scope_info> &method_info)
{
    auto &inner = cast.get_value();

    if (const auto var_ref = dynamic_cast<ast::var_reference::variable_reference_ast_node *>(&inner))
    {
        if (!variable_reference::resolve(context, *var_ref, method_info))
            return false;
    }
    else
    {
        if (!statement::resolve(context, inner, method_info))
            return false;
    }

    if (inner.get_type() == nullptr)
        return false;

    if (const auto custom_target = dynamic_cast<ast::type::custom_type_ast_node *>(cast.get_type()))
    {
        if (!util::resolve_type_node(context, *custom_target))
            return false;
    }

    if (is_primitive(cast.get_value().get_type()) != is_primitive(cast.get_type()))
    {
        context.throw_blasphemy(
            fmt::format(
                blasphemy::details::INVALID_CAST,
                cast.get_value().get_type()->to_pretty_string(),
                cast.get_type()->to_pretty_string()
            ),
            cast.get_code_position()
        );
        return false;
    }

    if (!is_primitive(cast.get_value().get_type()))
        return is_valid_reference_cast(context, cast);

    return true;
}

static bool is_valid_reference_cast(const semantic_context &context,
        codesh::ast::op::assignment::cast_ast_node &cast)
{
    if (const auto manual_cast = dynamic_cast<codesh::ast::op::assignment::manual_cast_ast_node *>(&cast))
    {
        // Needed for manual casts' IR generation
        manual_cast->set_is_upcast(
            util::can_poly_cast_to(context, *cast.get_value().get_type(), *cast.get_type())
        );
    }

    const auto *source_custom = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(cast.get_value().get_type());
    const auto *target_custom = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(cast.get_type());

    // If either type is not a resolved custom type, castability cannot be statically determined.
    // Just allow it.
    if (!is_custom_type(cast.get_value().get_type()) || !is_custom_type(cast.get_type()) ||
        !source_custom->is_resolved() || !target_custom->is_resolved())
    {
        return true;
    }

    const auto &source_sym = source_custom->get_resolved();
    const auto &target_sym = target_custom->get_resolved();

    const bool related = util::is_subtype_of(context, source_sym, target_sym) ||
        util::is_subtype_of(context, target_sym, source_sym);

    if (!related && !is_interface(source_sym) && !is_interface(target_sym))
    {
        context.throw_blasphemy(
            fmt::format(
                codesh::blasphemy::details::INVALID_CAST,
                cast.get_value().get_type()->to_pretty_string(),
                cast.get_type()->to_pretty_string()
            ),
            cast.get_code_position()
        );
        return false;
    }

    return true;
}

static bool is_primitive(const codesh::ast::type::type_ast_node *type)
{
    return dynamic_cast<const codesh::ast::type::primitive_type_ast_node *>(type) != nullptr;
}

static bool is_interface(const type_symbol &sym)
{
    return dynamic_cast<interface_declaration_ast_node *>(sym.get_producing_node()) != nullptr;
}

static bool is_custom_type(const codesh::ast::type::type_ast_node *type)
{
    return dynamic_cast<const codesh::ast::type::custom_type_ast_node *>(type) != nullptr;
}
