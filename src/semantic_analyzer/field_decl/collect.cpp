#include "collect.h"

#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/type_declaration/field_declaration_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"


void codesh::semantic_analyzer::field_declaration::collect(const semantic_context &context,
        country_symbol &country)
{
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const std::string name = type_decl->get_last_name(false);

        const auto type_symbol_raw = country.get_scope().resolve_local(name);
        if (!type_symbol_raw.has_value())
            continue;

        auto &type_sym = *static_cast<type_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            &type_symbol_raw->get()
        );

        for (const auto &field_decl : type_decl->get_fields())
        {
            type_sym.get_field_scope().add_symbol(
                field_decl->get_name(),
                std::make_unique<field_symbol>(
                    &type_sym,

                    type_sym.get_full_name().with(field_decl->get_name()),
                    field_decl->get_attributes()->clone(),
                    field_decl->get_type()->clone(),

                    field_decl.get()
                )
            );
        }
    }
}
