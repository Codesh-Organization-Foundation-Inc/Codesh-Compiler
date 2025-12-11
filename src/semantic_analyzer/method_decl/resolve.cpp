#include "resolve.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/custom_type_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "../util.h"

#include <functional>
#include <ranges>

static void resolve_return_type(const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method_decl);

static void resolve_parameters(const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method_decl);

static void resolve_local_variables(const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method_decl);


static const std::function<
        void(const codesh::ast::compilation_unit_ast_node &root,
             const codesh::ast::method::method_declaration_ast_node &method_decl)
    > METHOD_RESOLVERS[] = {
    resolve_return_type,
    resolve_parameters,
    resolve_local_variables,
};


void codesh::semantic_analyzer::method_declaration::resolve_methods(const ast::compilation_unit_ast_node &root) {
    for (const auto &type_decl : root.get_type_declarations())
    {
        const auto *class_node = dynamic_cast<ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_node)
            continue;

        for (const auto &method_decl : class_node->get_all_methods())
        {
            // Save the original descriptor.
            // This will be changed when applying the resolvers.
            const std::string original_descriptor = method_decl->generate_parameters_descriptor(false);

            for (const auto &resolver : METHOD_RESOLVERS)
            {
                try
                {
                    resolver(root, *method_decl);
                }
                catch (const std::runtime_error &e)
                {
                    std::ostringstream builder;

                    builder << e.what()
                        << " in method " << method_decl->get_name()
                        << " of type " << class_node->get_name();

                    collect_error(builder.str());
                }
            }


            // Move to a new overloads entry now that the parameters' descriptors are real:
            const auto &method_overloads = static_cast<method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                method_decl->get_symbol().get_parent_symbol()
            );

            // Get relevant method symbol from the method overloads map
            // Then cast it to method_symbol
            std::unique_ptr<method_symbol> method(
                static_cast<method_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                    method_overloads->resolve_and_move(original_descriptor).release()
                )
            );

            method_overloads->add_symbol(method_decl->generate_parameters_descriptor(), std::move(method));
        }

    }
}


static void resolve_return_type(const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
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

    if (!codesh::semantic_analyzer::util::resolve_custom_type_node(
        lookup_countries,
        *return_type,
        method_decl.get_symbol().get_return_type()
    )) {
        throw std::runtime_error("Unknown return type " + return_type->get_name());
    }
}

static void resolve_parameters(const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
    //TODO: Use actual countries
    const std::vector lookup_countries = {
        root.get_symbol_table().value().get().resolve_country("").value()
    };

    size_t i = 0;
    for (const auto &param : method_decl.get_parameters())
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(param->get_type());
        if (!custom_param)
            continue;

        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(
            lookup_countries,
            *custom_param,
            *method_decl.get_symbol().get_parameter_types()[i]
        )) {
            throw std::runtime_error("Unknown return type " + custom_param->get_name());
        }

        ++i;
    }
}

static void resolve_local_variables(const codesh::ast::compilation_unit_ast_node &root,
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
    //TODO: Use actual countries
    const std::vector lookup_countries = {
        root.get_symbol_table().value().get().resolve_country("").value()
    };

    for (const auto &var_symbol : method_decl.get_symbol().get_scope().get_variables() | std::views::values)
    {
        auto *custom_param = dynamic_cast<codesh::ast::type::custom_type_ast_node *>(var_symbol->get_type());
        if (!custom_param)
            continue;

        if (!codesh::semantic_analyzer::util::resolve_custom_type_node(
            lookup_countries,
            *custom_param
        )) {
            throw std::runtime_error("Unknown type " + custom_param->get_name());
        }
    }
}
