#include "check_types.h"
#include "../errors/errors.h"
#include <unordered_set>

namespace codesh::semantic_analyzer
{

void checkTypes(ast::compilation_unit_ast_node& root)
{
    std::unordered_set<std::string> typeNames;

    for (auto& typeDecl : root.get_type_declarations())
    {
        const std::string& name = typeDecl->get_name();

        if (typeNames.contains(name))
        {
            throw_error("Duplicate type declared: " + name);
        }

        typeNames.insert(name);
    }
}

}
