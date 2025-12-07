#include "collect.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../method_decl/collect.h"

#include "../../blasphemies/blasphemy_collector.h"

void codesh::semantic_analyzer::type_declaration::collect_types(const ast::compilation_unit_ast_node &root_node,
        country_symbol &containing_country)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const std::string name = type_decl->get_name();

        const auto [it, inserted] = containing_country.add_symbol(
            name, std::make_unique<type_symbol>(
                type_decl->get_attributes()->get_access_flags(),
                type_decl.get()
            )
        );

        if (!inserted)
        {
            error::get_blasphemy_collector().add_blasphemy(
            "נֵאִיפַת עֶצֶם תִּהְיֶה: כִּי הָעֶצֶם " + name + " מֻגְדָּר מְסַפֵּר פְּעָמִים בְּאוֹתוֹ עַמּוּד",
                error::blasphemy_type::SEMANTIC);
        }


        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            method_declaration::collect_methods(*class_decl, it.get());
        }
    }
}
