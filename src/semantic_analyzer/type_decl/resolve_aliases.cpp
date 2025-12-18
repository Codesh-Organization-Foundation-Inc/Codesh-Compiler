#include "resolve_aliases.h"
#include "../method_decl/resolve_aliases.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../semantic_context.h"

void codesh::semantic_analyzer::type_declaration::resolve_aliases(const semantic_context &context,
                                                                  const country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_name();
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);


        type_symbol &type = *static_cast<type_symbol *>(&country.resolve(type_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)
        method_declaration::resolve_aliases(new_context, type);
    }
}
