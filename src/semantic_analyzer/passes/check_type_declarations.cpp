#include "../errors/errors.h"
#include "check_type_declarations.h"
#include <unordered_set>

void codesh::semantic_analyzer::check_type_declarations(ast::compilation_unit_ast_node &root)
{
    //FIXME: This should point to the symbol table
    std::unordered_set<std::string> type_names;

    for (const auto &type_decl : root.get_type_declarations())
    {
        const std::string &name = type_decl->get_name();
        const auto [it, did_insert] = type_names.emplace(name);

        if (!did_insert)
        {
            throw_error("Duplicate type declared: " + name);
        }
    }
}
