#include "resolve.h"

#include "blasphemy/details.h"
#include "lexer/source_file_info.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/constructor_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "semantic_analyzer/builtins.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "semantic_analyzer/util.h"
#include "semantic_analyzer/util/poly_util.h"

#include <fmt/format.h>
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

static void resolve_thrown_exceptions(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl);


void codesh::semantic_analyzer::method_declaration::resolve(
    const semantic_context &context, const type_symbol &type,
    const ast::method::method_declaration_ast_node &method_decl)
{
    const auto new_context = context.with_consumer("בְּמַעֲשֶׂה", method_decl.to_pretty_string());
    resolve_method_signature(new_context, method_decl, type);

    // For constructors, also resolve the constructed type:
    if (const auto constructor_decl = dynamic_cast<const ast::method::constructor_declaration_ast_node *>(&method_decl))
    {
        util::resolve_type_node(
            context,
            constructor_decl->get_constructed_type()
        );
    }
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
    resolve_thrown_exceptions(context, method_decl);

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
    for (const auto &var_symbol : method_symbol.get_all_local_variables().name_to_var | std::views::values)
    {
        codesh::semantic_analyzer::util::resolve_type_node(
            context,
            *var_symbol.get().get_type(),
            *var_symbol.get().get_producing_node()->get_type()
        );
    }
}

static void resolve_thrown_exceptions(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::method_declaration_ast_node &method_decl)
{
    const auto &exceptions = method_decl.get_exceptions_thrown();
    if (exceptions.empty())
        return;

    const auto het = context.symbol_table_
        .get_talmud_codesh_country()
        .resolve_own(codesh::semantic_analyzer::builtins::ALIAS_EXCEPTION)
        .value();

    for (const auto &exception_node : exceptions)
    {
        const auto resolved = codesh::semantic_analyzer::util::resolve_custom_type_node(
            context, *exception_node
        );
        if (!resolved.has_value())
            continue;

        const auto &resolved_type = resolved->get();

        const bool valid = codesh::semantic_analyzer::util::is_subtype_of(
            resolved_type,
            static_cast<const codesh::semantic_analyzer::type_symbol &>(het.get()) // NOLINT(*-pro-type-static-cast-downcast)
        );

        if (!valid)
        {
            context.throw_blasphemy(
                fmt::format(
                    codesh::blasphemy::details::THROWS_NOT_EXCEPTION,
                    exception_node->get_unresolved_name().join()
                ),
                exception_node->get_code_position()
            );
        }
    }
}
