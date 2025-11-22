#include "type_declaration.h"

#include "../errors/errors.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "method_declaration.h"

void codesh::semantic_analyzer::type_declaration::collect_types(const ast::compilation_unit_ast_node &root_node,
        country_symbol &containing_country)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const std::string name = type_decl->get_name();

        const auto insert_result = containing_country.add_symbol(
            name, std::make_unique<type_symbol>(
                containing_country,
                type_decl->get_attributes()->get_access_flags()
            )
        );

        if (!insert_result)
        {
            collect_error("Duplicate type declared: " + name);
        }


        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            collect_methods(*class_decl, insert_result.value().get());
        }
    }
}
