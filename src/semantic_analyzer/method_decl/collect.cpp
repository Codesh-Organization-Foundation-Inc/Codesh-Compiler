#include "collect.h"

#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "../errors/errors.h"
#include "../util.h"

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl);

static void collect_local_variables(codesh::ast::method::method_declaration_ast_node &method_decl,
                                    codesh::semantic_analyzer::method_symbol &method_symbol);


void codesh::semantic_analyzer::method_declaration::collect_methods(
        const ast::type_decl::class_declaration_ast_node &class_decl, type_symbol &containing_type)
{
    for (const auto &method_decl : class_decl.get_all_methods())
    {
        method_overloads_symbol &methods_container =
            util::get_method_overloads_symbol(method_decl->get_name(), containing_type);

        const auto [it, inserted] = methods_container.add_symbol(
            method_decl->generate_parameters_descriptor(false), std::make_unique<method_symbol>(
                &methods_container,
                method_decl->get_attributes()->get_access_flags(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone(),
                method_decl.get()
            )
        );

        if (!inserted)
        {
            //TODO: Print full method declaration
            std::ostringstream builder;
            builder << "Duplicate method declared: " << method_decl->get_name();

            collect_error(builder.str());
        }

        method_decl->set_symbol(it);


        collect_local_variables(*method_decl, it);
    }
}

static void collect_local_variables(codesh::ast::method::method_declaration_ast_node &method_decl,
                                    codesh::semantic_analyzer::method_symbol &method_symbol)
{
    // Parameters
    for (const auto &param : method_decl.get_parameters())
    {
        auto &method_scope = method_symbol.get_scope();

        method_scope.get_variables().emplace(
            param->get_name(),
            std::make_unique<codesh::semantic_analyzer::local_variable_symbol>(
                &method_scope,
                param->get_type()->clone()
            )
        );
    }
}

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> result;

    for (const auto &param_node : method_decl.get_parameters())
    {
        result.push_back(param_node->get_type()->clone());
    }

    return result;
}

