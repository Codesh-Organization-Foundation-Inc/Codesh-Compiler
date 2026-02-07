#include "util.h"

#include "blasphemy/details.h"
#include "fmt/compile.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_context.h"
#include "symbol_table/symbol_table.h"

std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> codesh::semantic_analyzer::util::
    resolve_custom_type(const semantic_context &context, const ast::type::custom_type_ast_node &custom_type_node)
{
    // If we already know the type declaration, no need to find it.
    if (const auto known_decl = custom_type_node.get_known_type_declaration())
    {
        return known_decl->get().get_resolved();
    }


    const auto &full_name = custom_type_node.get_unresolved_name();

    const auto result_raw = symbol_table::resolve_from_imports(
        context,
        full_name,
        blasphemy::NO_CODE_POS
    );

    if (!result_raw.has_value())
        return std::nullopt;

    const auto result = dynamic_cast<type_symbol *>(&result_raw->get());
    if (!result)
    {
        context.blasphemy_consumer(fmt::format(
            blasphemy::details::NOT_AN_OBJECT,
            full_name.holy_join()
        ), blasphemy::NO_CODE_POS);
        return std::nullopt;
    }

    return *result;
}

bool codesh::semantic_analyzer::util::resolve_custom_type_node(const semantic_context &context,
        ast::type::custom_type_ast_node &custom_type_node,
        const std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node)
{
    const auto result = resolve_custom_type(
        context,
        custom_type_node
    );

    if (!result.has_value())
        return false;


    custom_type_node.set_resolved(result.value());

    if (related_type_node.has_value())
    {
        auto &custom_type = *static_cast<ast::type::custom_type_ast_node *>(&related_type_node->get()); // NOLINT(*-pro-type-static-cast-downcast)
        custom_type.set_resolved(result.value());
    }

    return true;
}

bool codesh::semantic_analyzer::util::resolve_type_node(
    const semantic_context &context, ast::type::type_ast_node &type_node,
    const std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node)
{
    auto *custom_type = dynamic_cast<ast::type::custom_type_ast_node *>(&type_node);
    if (!custom_type)
        return true;

    return resolve_custom_type_node(
        context,
        *custom_type,
        related_type_node
    );
}

bool codesh::semantic_analyzer::util::are_types_compatible(const ast::type::type_ast_node &from,
                                                           const ast::type::type_ast_node &to)
{
    //FIXME: Should check for auto conversions (and convert!) if not an exact match.
    return from.generate_descriptor() == to.generate_descriptor();
}

codesh::semantic_analyzer::method_overloads_symbol &codesh::semantic_analyzer::util::get_method_overloads_symbol(
        const std::string &name, type_symbol &containing_type)
{
    return containing_type.get_scope().add_symbol(
        name, std::make_unique<method_overloads_symbol>(&containing_type)
    ).first;
}