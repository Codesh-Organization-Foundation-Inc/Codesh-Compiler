#include "collect.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../method_decl/collect.h"

#include "../semantic_context.h"
#include "../../blasphemy/blasphemy_collector.h"

void codesh::semantic_analyzer::type_declaration::collect(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_unresolved_name().get_last_part();
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);


        const auto [it, inserted] = country.add_symbol(
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

        method_declaration::collect_methods(new_context, *type_decl, it.get());
    }
}
