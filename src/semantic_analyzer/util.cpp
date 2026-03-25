#include "util.h"

#include "blasphemy/details.h"
#include "defenition/fully_qualified_name.h"
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

    const auto result_raw = context.symbol_table_.resolve(
        context,
        full_name
    );

    if (!result_raw.has_value())
        return std::nullopt;

    const auto result = dynamic_cast<type_symbol *>(&result_raw->get());
    if (!result)
    {
        context.throw_blasphemy(fmt::format(
            blasphemy::details::NOT_AN_OBJECT,
            full_name.holy_join()
        ), full_name.get_source_range());
        return std::nullopt;
    }

    return *result;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> codesh::semantic_analyzer::util::
    resolve_custom_type_node(const semantic_context &context, ast::type::custom_type_ast_node &custom_type_node,
        const std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node)
{
    if (custom_type_node.is_resolved())
        return custom_type_node.get_resolved();

    const auto result = resolve_custom_type(
        context,
        custom_type_node
    );

    if (!result.has_value())
        return std::nullopt;


    custom_type_node.set_resolved(result.value());

    if (related_type_node.has_value())
    {
        auto &custom_type = *static_cast<ast::type::custom_type_ast_node *>(&related_type_node->get()); // NOLINT(*-pro-type-static-cast-downcast)
        custom_type.set_resolved(result.value());
    }

    return result;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> codesh::semantic_analyzer::util::
    resolve_custom_type_node(const semantic_context &context, ast::type::type_ast_node &type_node,
                             const std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node)
{
    auto *custom_type = dynamic_cast<ast::type::custom_type_ast_node *>(&type_node);
    if (!custom_type)
        return std::nullopt;

    return resolve_custom_type_node(
        context,
        *custom_type,
        related_type_node
    );
}

bool codesh::semantic_analyzer::util::resolve_type_node(const semantic_context &context,
        ast::type::type_ast_node &type_node,
        const std::optional<std::reference_wrapper<ast::type::type_ast_node>> related_type_node)
{
    auto *custom_type = dynamic_cast<ast::type::custom_type_ast_node *>(&type_node);
    if (!custom_type)
        return true;

    return resolve_custom_type_node(
        context,
        *custom_type,
        related_type_node
    ).has_value();
}

bool codesh::semantic_analyzer::util::do_types_match(const ast::type::type_ast_node &from,
                                                     const ast::type::type_ast_node &to)
{
    return from.generate_descriptor() == to.generate_descriptor();
}

codesh::semantic_analyzer::method_overloads_symbol &codesh::semantic_analyzer::util::get_method_overloads_symbol(
        const std::string &name, type_symbol &containing_type)
{
    return containing_type.get_scope().add_symbol(
        name, std::make_unique<method_overloads_symbol>(&containing_type)
    ).first;
}

codesh::semantic_analyzer::country_symbol &codesh::semantic_analyzer::util::find_or_create_country(
        const symbol_table &table, const definition::fully_qualified_name &fqn)
{
    country_symbol &root = table.get_global_country();

    const auto &parts = fqn.get_parts();
    if (parts.empty())
        return root;

    country_symbol *current = &root;

    for (size_t i = 0; i < parts.size(); ++i)
    {
        const auto &part = parts[i];

        const auto existing = current->get_scope().resolve_local(part);
        if (existing)
        {
            current = &dynamic_cast<country_symbol &>(existing->get());
        }
        else
        {
            definition::fully_qualified_name sub_fqn(
                lexer::NO_CODE_POS,
                parts.cbegin(),
                parts.cbegin() + static_cast<std::ptrdiff_t>(i) + 1
            );

            current = &current->get_scope().add_symbol(
                part,
                std::make_unique<country_symbol>(std::move(sub_fqn), current)
            ).first.get();
        }
    }

    return *current;
}

std::pair<
    std::reference_wrapper<codesh::semantic_analyzer::type_symbol>,
    bool
> codesh::semantic_analyzer::util::add_type_symbol(country_symbol &country, const std::string &name,
        std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::unique_ptr<ast::type::custom_type_ast_node> super_type,
        std::vector<std::unique_ptr<ast::type::custom_type_ast_node>> interfaces,
        ast::type_decl::type_declaration_ast_node *decl)
{
    return country.get_scope().add_symbol(
        name,
        std::make_unique<type_symbol>(
            &country,
            country.get_full_name().with(name),
            std::move(super_type),
            std::move(interfaces),
            std::move(attributes),
            decl
        )
    );
}

codesh::semantic_analyzer::field_symbol &codesh::semantic_analyzer::util::add_field_symbol(type_symbol &type_sym,
        const std::string &name, std::unique_ptr<ast::type_decl::attributes_ast_node> attributes,
        std::unique_ptr<ast::type::type_ast_node> type)
{
    return type_sym.get_field_scope().add_symbol(
        name,
        std::make_unique<field_symbol>(
            &type_sym,
            type_sym.get_full_name().with(name),
            std::move(attributes),
            std::move(type),
            nullptr
        )
    ).first.get();
}