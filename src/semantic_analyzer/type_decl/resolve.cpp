#include "resolve.h"
#include "semantic_analyzer/method_decl/resolve.h"

#include "fmt/xchar.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"

void codesh::semantic_analyzer::type_declaration::resolve(const semantic_context &context, const country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_last_name(false);
        const semantic_context new_context = context.with_consumer("בָּעֶצֶם", name);

        const type_symbol &type = *static_cast<type_symbol *>(&country.get_scope().resolve_local(name).value().get()); // NOLINT(*-pro-type-static-cast-downcast)


        for (const auto &method_decl : type_decl->get_all_methods())
        {
            method_declaration::resolve(new_context, type, *method_decl);
        }
    }
}
