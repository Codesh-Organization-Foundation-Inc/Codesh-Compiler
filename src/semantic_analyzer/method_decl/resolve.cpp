#include "resolve.h"

#include "blasphemy/blasphemy_collector.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/util.h"

#include <functional>
#include <ranges>

static void resolve_return_type(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl,
        const codesh::semantic_analyzer::method_symbol &method_symbol);

static void resolve_local_variables(const codesh::semantic_analyzer::semantic_context &context,
                                    const codesh::semantic_analyzer::method_symbol &method_symbol);

static codesh::semantic_analyzer::method_symbol &resolve_method_signature(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl,
        const codesh::semantic_analyzer::type_symbol &type);


void codesh::semantic_analyzer::method_declaration::resolve(
    const semantic_context &context, const type_symbol &type,
    const ast::method::method_declaration_ast_node &method_decl)
{
    const auto new_context = context.with_consumer("בְּמַעֲשֶׂה", method_decl.get_last_name(false));
    resolve_method_signature(new_context, method_decl, type);
}

static codesh::semantic_analyzer::method_symbol &resolve_method_signature(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl,
        const codesh::semantic_analyzer::type_symbol &type)
{
    auto &method_overloads = *static_cast<codesh::semantic_analyzer::method_overloads_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
        &type.get_scope().resolve_local(method_decl.get_last_name(false)).value().get()
    );

    // Get relevant method symbol from the method overloads map
    // Then cast it to method_symbol
    std::unique_ptr<codesh::semantic_analyzer::method_symbol> method(
        static_cast<codesh::semantic_analyzer::method_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
            method_overloads.get_scope()
                .resolve_and_move(method_decl.generate_parameters_descriptor(false))
                .release()
        )
    );

    resolve_return_type(context, method_decl, *method);
    resolve_local_variables(context, *method);

    // Move to a new overloads entry, now that the parameters' descriptors are valid
    const auto insert_result =
        method_overloads.get_scope().add_symbol(method_decl.generate_parameters_descriptor(), std::move(method));

    return insert_result.first.get();
}

static void resolve_return_type(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl,
        const codesh::semantic_analyzer::method_symbol &method_symbol)
{
    codesh::semantic_analyzer::util::resolve_type_node(
        context,
        *method_decl.get_return_type(),
        method_symbol.get_return_type()
    );
}

static void resolve_local_variables(const codesh::semantic_analyzer::semantic_context &context,
                                    const codesh::semantic_analyzer::method_symbol &method_symbol)
{
    for (const auto &var_symbol : method_symbol.get_all_local_variables() | std::views::values)
    {
        codesh::semantic_analyzer::util::resolve_type_node(
            context,
            *var_symbol.get().get_type(),
            *var_symbol.get().get_producing_node()->get_type()
        );
    }
}
