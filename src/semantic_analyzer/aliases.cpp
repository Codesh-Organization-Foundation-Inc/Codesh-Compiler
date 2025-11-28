#include "aliases.h"

#include "../parser/ast/compilation_unit_ast_node.h"
#include "passes/method_declaration.h"
#include "symbol_table/symbol.h"

void codesh::semantic_analyzer::resolve_aliases(const ast::compilation_unit_ast_node &ast_root)
{
    //TODO: Properly resolve country of origin
    const country_symbol &country = ast_root.get_symbol_table()->get().resolve_country("").value();

    for (const auto &type_decl : ast_root.get_type_declarations())
    {
        auto *class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_node)
            continue;

        type_symbol &type = *static_cast<type_symbol *>(&country.resolve(type_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)
        method_declaration::resolve_aliases(type);
    }
}
