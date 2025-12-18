#include "resolve.h"
#include "../method_decl/resolve.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../semantic_context.h"
#include "fmt/xchar.h"

void codesh::semantic_analyzer::type_declaration::resolve(const semantic_context &context)
{
    //TODO: Properly get country of origin
    const country_symbol &country = context.root.get_symbol_table()->get().resolve_country("").value();

    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_name();

        blasphemy::blasphemy_consumer type_bc = [&context, &name](std::string details) {
            context.blasphemy_consumer(fmt::format(
                "{} בָּעֶצֶם {}",
                std::move(details),
                name
            ));
        };

        const type_symbol &type = *static_cast<type_symbol *>(&country.resolve(name).value().get()); // NOLINT(*-pro-type-static-cast-downcast)


        for (const auto &method_decl : type_decl->get_all_methods())
        {
            method_declaration::resolve(context.with_consumer(type_bc), type, *method_decl);
        }
    }
}
