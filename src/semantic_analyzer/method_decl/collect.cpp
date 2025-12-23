#include "collect.h"

#include "../../blasphemy/blasphemy_collector.h"
#include "../../parser/ast/compilation_unit_ast_node.h"
#include "../../parser/ast/type/primitive_type_ast_node.h"
#include "../semantic_context.h"
#include "../util.h"

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl);

static void collect_local_variables(codesh::ast::method::method_declaration_ast_node &method_decl,
                                    codesh::semantic_analyzer::method_symbol &method_symbol);


void codesh::semantic_analyzer::method_declaration::collect_methods(const semantic_context &context,
        const ast::type_decl::type_declaration_ast_node &type_decl, type_symbol &containing_type)
{
    for (const auto &method_decl : type_decl.get_all_methods())
    {
        const std::string method_name = method_decl->get_last_name(false);
        const auto new_context = context.with_consumer("בְּמַעֲשֶׂה", method_decl->get_last_name(false));

        method_overloads_symbol &methods_container = util::get_method_overloads_symbol(method_name, containing_type);

        const auto [it, inserted] = methods_container.add_symbol(
            method_decl->generate_parameters_descriptor(false), std::make_unique<method_symbol>(
                &methods_container,
                containing_type,
                containing_type.get_full_name().with(method_name),

                method_decl->get_attributes()->clone(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone(),

                method_decl.get()
            )
        );

        if (!inserted)
        {
            //TODO: Print full method declaration
            new_context.blasphemy_consumer(fmt::format(
                "נֵאִיפַה: הֻכְרַז מַעֲשֶׂה כָּפוּל: {}",
                method_name
            ));
        }

        method_decl->set_resolved(it);


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

        method_scope.add_variable(
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

