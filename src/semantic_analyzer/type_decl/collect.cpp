#include "collect.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../method_decl/collect.h"

#include "../semantic_context.h"
#include "../../blasphemy/blasphemy_collector.h"

void codesh::semantic_analyzer::type_declaration::collect_types(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_name();
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);


        const auto [it, inserted] = country.add_symbol(
            name, std::make_unique<type_symbol>(
                &country,
                type_decl->get_attributes()->get_access_flags(),
                type_decl.get()
            )
        );

        if (!inserted)
        {
            new_context.blasphemy_consumer(fmt::format(
                "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד",
                name
            ));
        }


        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            method_declaration::collect_methods(new_context, *class_decl, it.get());
        }
    }
}
