#include "check_types.h"
#include "../errors/errors.h"
#include <unordered_set>

void codesh::semantic_analyzer::check_types(ast::compilation_unit_ast_node &root)
{
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
