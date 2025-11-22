#include "../errors/errors.h"
#include "check_type_declarations.h"

#include "../symbol_table/symbol_table.h"

//TODO: Move to inner method of symbol table, make it collect everything.
void codesh::semantic_analyzer::add_type_declarations(ast::compilation_unit_ast_node &root_node)
{
    //TODO: Iterate over each and every existing country.
    // Same as the root node.
    auto &country = root_node.get_symbol_table().resolve_country("")->get();

    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const std::string name = type_decl->get_name();

        const bool did_insert = country.add_symbol(name, std::make_unique<type_symbol>(
            country,
            type_decl->get_attributes()->get_access_flags(),
            type_decl->generate_descriptor()
        ));

        if (!did_insert)
        {
            collect_error("Duplicate type declared: " + name);
        }
    }
}
