#include "method_declaration.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "util.h"

static codesh::semantic_analyzer::methods_overloads_symbol &get_method_overloads(
        const codesh::ast::method_declaration_ast_node &method_decl,
        codesh::semantic_analyzer::type_symbol &containing_type);

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method_declaration_ast_node &method_decl);

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


void codesh::semantic_analyzer::method_declaration::collect_methods(const ast::type_decl::class_declaration_ast_node &class_decl,
                                                type_symbol &containing_type)
{
    for (const auto &method_decl : class_decl.get_methods())
    {
        methods_overloads_symbol &methods_container = get_method_overloads(*method_decl, containing_type);

        const auto [it, inserted] = methods_container.add_symbol(
            method_decl->generate_parameter_descriptors(), std::make_unique<method_symbol>(
                methods_container,
                method_decl->get_attributes()->get_access_flags(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone()
            )
        );

        if (!inserted)
        {
            //TODO: Print full method declaration
            collect_error("Duplicate method declared: " + method_decl->get_name());
        }

        //TODO: Collect local variables
    }
}

static codesh::semantic_analyzer::methods_overloads_symbol &get_method_overloads(
        const codesh::ast::method_declaration_ast_node &method_decl,
        codesh::semantic_analyzer::type_symbol &containing_type)
{
    return containing_type.add_symbol(
        method_decl.get_name(), std::make_unique<codesh::semantic_analyzer::methods_overloads_symbol>(
            containing_type
        )
    ).first;
}

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method_declaration_ast_node &method_decl)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> result;

    for (const auto &param_node : method_decl.get_parameters())
    {
        result.push_back(param_node->get_type()->clone());
    }

    return result;
}


void codesh::semantic_analyzer::method_declaration::check_methods(const ast::compilation_unit_ast_node &root) {
    //TODO: Properly get country of origin
    const country_symbol &country = root.get_symbol_table().value().get().resolve_country("").value();

    for (const auto &type_decl : root.get_type_declarations())
    {
        auto *class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_node)
            continue;

        // Get type symbol
        const type_symbol &type = *static_cast<type_symbol *>(&country.resolve(type_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)

        for (const auto &method_decl : class_node->get_methods())
        {
            const method_symbol &method = *static_cast<method_symbol *>(&type.resolve(method_decl->get_name()).value().get()); // NOLINT(*-pro-type-static-cast-downcast)

            resolve_return_type(root, *method_decl, class_node->get_name());
            resolve_parameters(root, *method_decl, class_node->get_name());

            //TODO: Update method entry in the method scope table
            //method.
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
        codesh::semantic_analyzer::collect_error(
            "Unknown return type " + return_type->get_name() +
            " in method " + method_decl.get_name() +
            " of type " + class_name
        );
    }
}

static void resolve_parameters(
    const codesh::ast::compilation_unit_ast_node &root,
    const codesh::ast::method_declaration_ast_node &method,
    const std::string &class_name
) {
    for (const auto &param : method.get_parameters())
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param.get());

        if (!custom_param)
        {
            // Primitive types bound to be okay
            continue;
        }


        //TODO: Use actual countries
        const std::vector lookup_countries = {
            root.get_symbol_table().value().get().resolve_country("").value()
        };


        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(lookup_countries, *custom_param))
        {
            codesh::semantic_analyzer::collect_error(
                "Unknown return type " + custom_param->get_name() +
                " in method " + method.get_name() +
                " of type " + class_name
            );
        }
    }
}
