#include "resolve.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../../blasphemies/blasphemy_collector.h"
#include "../util.h"

static void resolve_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method_decl,
    const std::string &class_name
);

static void resolve_parameters(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method,
    const std::string &class_name
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

        for (const auto &method_decl : class_node->get_methods())
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

            resolve_return_type(root, *method_decl, class_node->get_name());
            resolve_parameters(root, *method_decl, class_node->get_name());


            // Move to a new overloads entry now that the parameters' descriptors are real
            method_overloads.add_symbol(method_decl->generate_parameters_descriptor(), std::move(method));
        }

    }
}


static void resolve_return_type(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method_decl,
    const std::string &class_name
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
        std::ostringstream os_string;
        os_string << "Unknown return type " << return_type->get_name()
            << " in method " << method_decl.get_name()
            << " of type " << class_name;

        codesh::error::get_blasphemy_collector().add_blasphemy(os_string.str(),
                                                               codesh::error::blasphemy_type::SEMANTIC);
    }
}

static void resolve_parameters(
        const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method_declaration_ast_node &method,
        const std::string &class_name)
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
            std::ostringstream os_string;
            os_string << "Unknown return type " << custom_param->get_name()
                << " in method " << method.get_name()
                << " of type " << class_name;

            codesh::error::get_blasphemy_collector().add_blasphemy(os_string.str(),
                                                                   codesh::error::blasphemy_type::SEMANTIC);
        }
    }
}
