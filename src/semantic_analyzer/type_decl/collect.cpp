#include "collect.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../method_decl/collect.h"

#include "../../blasphemy/blasphemy_collector.h"

void codesh::semantic_analyzer::type_declaration::collect_types(const ast::compilation_unit_ast_node &root_node,
        country_symbol &containing_country, const blasphemy::blasphemy_consumer &blasphemy_consumer)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const std::string name = type_decl->get_name();

        blasphemy::blasphemy_consumer type_bc = [&blasphemy_consumer, &name](std::string details) {
            blasphemy_consumer(fmt::format(
                "{} בָּעֶצֶם {}",
                std::move(details),
                name
            ));
        };


        const auto [it, inserted] = containing_country.add_symbol(
            name, std::make_unique<type_symbol>(
                &containing_country,
                type_decl->get_attributes()->get_access_flags(),
                type_decl.get()
            )
        );

        if (!inserted)
        {
            type_bc(fmt::format(
                "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי־מֻגְדָּר הָעֶצֶם {} מְסַפֵּר פְּעָמִים בְּאוֹתוֹ הַעַמּוּד",
                name
            ));
        }


        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            method_declaration::collect_methods(*class_decl, it.get(), type_bc);
        }
    }
}
