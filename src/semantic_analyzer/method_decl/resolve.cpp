#include "resolve.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "../util.h"

static void resolve_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method::method_declaration_ast_node &method_decl
);

static void resolve_parameters(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method::method_declaration_ast_node &method
);


void codesh::semantic_analyzer::method_declaration::resolve_methods(const ast::compilation_unit_ast_node &root) {
    //TODO: Properly get country of origin
    const country_symbol &country = root.get_symbol_table()->get().resolve_country("").value();

    for (const auto &type_decl : root.get_type_declarations())
    {
        auto *class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_node)
            continue;

        // Get type symbol
        const type_symbol &type = *static_cast<type_symbol *>(&country.resolve(type_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)

        for (const auto &method_decl : class_node->get_all_methods())
        {
            method_overloads_symbol &method_overloads = *static_cast<method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                &type.resolve(method_decl->get_name()).value().get()
            );

            std::unique_ptr<method_symbol> method(
                static_cast<method_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                    method_overloads.resolve_and_move(method_decl->generate_parameters_descriptor(false))
                        .release()
                )
            );

            try
            {
                resolve_return_type(root, *method_decl);
                resolve_parameters(root, *method_decl);
            }
            catch (const std::runtime_error &e)
            {
                std::ostringstream os_string;
                os_string << e.what()
                    << " in method " << method_decl->get_name()
                    << " of type " << class_node->get_name();

                collect_error(os_string.str());
            }

            // Move to a new overloads entry now that the parameters' descriptors are real
            method_overloads.add_symbol(method_decl->generate_parameters_descriptor(), std::move(method));
        }

    }
}


static void resolve_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method::method_declaration_ast_node &method_decl
) {
    auto *return_type = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(method_decl.get_return_type());

    if (!return_type)
    {
        // Primitive types bound to be okay
        return;
    }

    //TODO: Use actual countries
    const std::vector lookup_countries = {
        root.get_symbol_table().value().get().resolve_country("").value()
    };

    if (!codesh::semantic_analyzer::util::resolve_custom_type_node(lookup_countries, *return_type))
    {
        throw std::runtime_error("Unknown return type " + return_type->get_name());
    }
}

static void resolve_parameters(
        const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method)
{
    for (const auto &param : method.get_parameters())
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param->get_type());

        if (!custom_param)
            continue;

        //TODO: Use actual countries
        const std::vector lookup_countries = {
            root.get_symbol_table().value().get().resolve_country("").value()
        };

        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(lookup_countries, *custom_param))
        {
            throw std::runtime_error("Unknown return type " + custom_param->get_name());
        }
    }
}
