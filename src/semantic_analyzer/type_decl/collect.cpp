#include "collect.h"

#include "parser/ast/compilation_unit_ast_node.h"
#include "semantic_analyzer/method_decl/collect.h"
#include "semantic_analyzer/symbol_table/symbol.h"

#include "semantic_analyzer/semantic_context.h"
#include "blasphemy/blasphemy_collector.h"

void codesh::semantic_analyzer::type_declaration::collect_types(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_last_name(false);
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);


        const auto [it, inserted] = country.get_scope().add_symbol(
            name, std::make_unique<type_symbol>(
                &country,
                country.get_full_name().with(name),

                type_decl->get_attributes()->clone(),
                type_decl.get()
            )
        );

        if (!inserted)
        {
            new_context.blasphemy_consumer(fmt::format(
                "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד",
                type_decl->get_unresolved_name().join()
            ));
        }
    }
}

void codesh::semantic_analyzer::type_declaration::collect_methods(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_last_name(false);
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);

        const auto type_symbol_raw = country.get_scope().resolve_local(name);
        if (!type_symbol_raw.has_value())
            continue;

        auto &type_symbol = *static_cast<codesh::semantic_analyzer::type_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            &type_symbol_raw->get()
        );

        method_declaration::collect_methods(new_context, *type_decl, type_symbol);
    }
}

void codesh::semantic_analyzer::type_declaration::collect(const semantic_context &context,
        country_symbol &country)
{
    collect_types(context, country);
    collect_methods(context, country);
}
