#include "collect.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/util.h"

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl);

static void collect_local_variables(const codesh::ast::method::method_declaration_ast_node &method_decl,
                                    const codesh::semantic_analyzer::method_symbol &method_symbol);

void collect_inner_scopes(codesh::semantic_analyzer::method_symbol &method_symbol,
        const codesh::ast::method::method_scope_ast_node &current_scope_node,
        codesh::semantic_analyzer::method_scope_symbol &current_scope_symbol);

void codesh::semantic_analyzer::method_declaration::collect(const semantic_context &context,
        ast::method::method_declaration_ast_node &method_decl, type_symbol &containing_type)
{
    const std::string method_name = method_decl.get_last_name(false);
    const auto new_context = context.with_consumer("בְּמַעֲשֶׂה", method_decl.get_last_name(false));

    method_overloads_symbol &methods_container = util::get_method_overloads_symbol(method_name, containing_type);

    const auto [it, inserted] = methods_container.get_scope().add_symbol(
        method_decl.generate_parameters_descriptor(false), std::make_unique<method_symbol>(
            &methods_container,
            containing_type,
            containing_type.get_full_name().with(method_name),

            method_decl.get_attributes()->clone(),
            clone_parameter_types(method_decl),
            method_decl.get_return_type()->clone(),

            &method_decl
        )
    );

    if (!inserted)
    {
        //TODO: Print full method signature
        new_context.blasphemy_consumer(fmt::format(
            fmt::runtime(blasphemy::details::DUPLICATE_METHOD_DECLARATION),
            method_name
        ));
    }

    collect_local_variables(method_decl, it);

    collect_inner_scopes(
        it,
        method_decl.get_method_scope(),
        it.get().get_method_scope()
    );
}

static void collect_local_variables(const codesh::ast::method::method_declaration_ast_node &method_decl,
                                    const codesh::semantic_analyzer::method_symbol &method_symbol)
{
    //TODO: Handle inner scopes
    for (auto &var_decl : method_decl.get_method_scope().get_local_variables())
    {
        method_symbol.get_method_scope().add_variable(*var_decl);
    }
}

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> result;

    for (const auto &param_node : method_decl.get_parameters())
    {
        result.push_back(param_node.get().get_type()->clone());
    }

    return result;
}

void collect_inner_scopes(codesh::semantic_analyzer::method_symbol &method_symbol,
        const codesh::ast::method::method_scope_ast_node &current_scope_node,
        codesh::semantic_analyzer::method_scope_symbol &current_scope_symbol)
{
    for (const auto &inner_scope_node : current_scope_node.get_method_scopes())
    {
        auto &inner_scope_symbol = current_scope_symbol.add_inner_scope(
            method_symbol.create_method_scope(current_scope_symbol, *inner_scope_node)
        );

        // Recursively collect even more inner scopes
        for (const auto &inner_inner_scope : inner_scope_node->get_method_scopes())
        {
            collect_inner_scopes(method_symbol, *inner_inner_scope, inner_scope_symbol);
        }
    }
}
