#include "collect.h"

#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/method_decl/collect.h"
#include "semantic_analyzer/symbol_table/symbol.h"

#include "blasphemy/blasphemy_collector.h"
#include "parser/ast/type_declaration/error_type_declaration_ast_node.h"
#include "semantic_analyzer/semantic_context.h"

void codesh::semantic_analyzer::type_declaration::collect(const semantic_context &context,
        ast::type_decl::type_declaration_ast_node &type_decl, country_symbol &country)
{
    // No reason to collect error nodes
    if (dynamic_cast<const ast::type_decl::error_type_declaration_ast_node *>(&type_decl))
        return;

    const std::string name = type_decl.get_last_name(false);
    const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);


    const auto [it, inserted] = country.get_scope().add_symbol(
        name, std::make_unique<type_symbol>(
            &country,
            country.get_full_name().with(name),

            type_decl.get_attributes()->clone(),
            &type_decl
        )
    );

    if (!inserted)
    {
        new_context.blasphemy_consumer(fmt::format(
            "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד",
            type_decl.get_unresolved_name().holy_join()
        ), type_decl.get_code_position());
    }
}

void codesh::semantic_analyzer::type_declaration::collect_inheritance(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        if (dynamic_cast<const ast::type_decl::error_type_declaration_ast_node *>(type_decl.get()))
            continue;

        const std::string name = type_decl->get_last_name(false);
        type_symbol &type_sym = *static_cast<type_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            &country.get_scope().resolve_local(name).value().get()
        );

        if (const ast::type::custom_type_ast_node *super_node = type_decl->get_super_class())
        {
            const std::string super_name = super_node->get_unresolved_name().get_parts().back();
            const auto result = country.get_scope().resolve_local(super_name);

            if (result.has_value())
            {
                if (auto *super_sym = dynamic_cast<type_symbol *>(&result->get()))
                    type_sym.set_super_type(super_sym);
            }
        }
    }
}

void codesh::semantic_analyzer::type_declaration::dispatch_collect_methods(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_last_name(false);
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);

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
