#include "collect.h"

#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/method_decl/collect.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/util.h"

#include "lexer/source_file_info.h"
#include "parser/ast/type_declaration/error_type_declaration_ast_node.h"
#include "semantic_analyzer/semantic_context.h"


void codesh::semantic_analyzer::type_declaration::collect(const semantic_context &context,
        ast::type_decl::type_declaration_ast_node &type_decl, country_symbol &country)
{
    // No reason to collect error nodes
    if (dynamic_cast<const ast::type_decl::error_type_declaration_ast_node *>(&type_decl))
        return;

    const auto name = type_decl.get_last_name(false);
    const auto new_context = context.with_consumer("בָּעֶצֶם", name);

    const auto [it, inserted] = util::add_type_symbol(
        country,
        name,
        type_decl.get_attributes()->clone(),
        std::unique_ptr<ast::type::custom_type_ast_node>(
            static_cast<ast::type::custom_type_ast_node *>(type_decl.get_super_class()->clone().release()) // NOLINT(*-pro-type-static-cast-downcast)
        ),
        type_decl.get_interfaces_copy(),
        &type_decl
    );

    if (!inserted)
    {
        new_context.throw_blasphemy(fmt::format(
            "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד",
            type_decl.get_unresolved_name().holy_join()
        ), type_decl.get_name_range());
    }
}


void codesh::semantic_analyzer::type_declaration::dispatch_collect_methods(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const auto name = type_decl->get_last_name(false);
        const auto new_context = context.with_consumer("בָּעֶצֶם", name);

        const auto type_symbol_raw = country.get_scope().resolve_local(name);
        if (!type_symbol_raw.has_value())
            continue;

        auto &type_symbol = *static_cast<semantic_analyzer::type_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            &type_symbol_raw->get()
        );

        for (const auto &method_decl : type_decl->get_all_methods())
        {
            method_declaration::collect(new_context, *method_decl, type_symbol);
        }
    }
}
