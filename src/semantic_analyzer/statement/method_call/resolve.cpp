#include "resolve.h"

#include "blasphemy/blasphemy_collector.h"
#include "semantic_analyzer/statement/resolve.h"
#include "semantic_analyzer/statement/variable_reference/resolve.h"

#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "semantic_analyzer/util.h"
#include "blasphemy/details.h"

#include <ranges>

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_call(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        codesh::ast::method::operation::method_call_ast_node &method_call);

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> get_called_method_as_symbol(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        const codesh::ast::method::operation::method_call_ast_node &method_call);

/**
 * @returns Whether all arguments were successfully resolved.
 */
static bool resolve_arguments(const codesh::semantic_analyzer::semantic_context &context,
                              const codesh::ast::method::operation::method_call_ast_node &method_call_node,
                              const codesh::semantic_analyzer::method_symbol &containing_method,
                              const codesh::semantic_analyzer::method_scope_symbol &scope);


bool codesh::semantic_analyzer::statement::method_call::resolve(const semantic_context &context,
                                                     ast::method::operation::method_call_ast_node &method_call,
                                                     const method_symbol &containing_method,
                                                     const method_scope_symbol &scope)
{
    // We must first resolve the method's arguments
    // if we want to determine which argument types we pass forward (overloading)
    if (!resolve_arguments(context, method_call, containing_method, scope))
        return false;

    const auto result = resolve_method_call(
        context,
        containing_method,
        method_call
    );

    if (!result.has_value())
        return false;


    //TODO: Remove this once Talmud Codesh implements this method by itself:
    // Manually pass System.out to every מסוף ל־אמר call
    if (method_call.get_unresolved_name().join() == "מסוף/אמר")
    {
        auto system_in_reference = std::make_unique<variable_reference_ast_node>(blasphemy::NO_CODE_POS, "מסוף/פלט");
        system_in_reference->set_resolved(
            *static_cast<field_symbol *>( // NOLINT(*-pro-type-static-cast-downcast)
                &symbol_table::resolve_from_imports(
                    context, "מסוף/פלט",
                    method_call.get_code_position()
                )->get()
            )
        );

        method_call.get_arguments().push_front(std::move(system_in_reference));
    }

    return true;
}


static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_call(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto &name = method_call.get_unresolved_name();
    const codesh::semantic_analyzer::type_symbol *parent_type = nullptr;

    if (name.is_single_part())
    {
        // Since this is a single-part FQN (name only), the method must either be the classes' or a static import.
        //TODO: Handle static imports

        parent_type = &containing_method.get_parent_type();
    }
    else
    {
        const auto type_symbol = codesh::semantic_analyzer::symbol_table::resolve_from_imports(
            context,
            name,
            method_call.get_code_position(),
            // Ignore the last part of the name, which points to the method overloads.
            // get_called_method_as_symbol already handles it.
            name.get_parts().end() - 1
        );

        if (!type_symbol.has_value())
            return std::nullopt;

        // A method must be contained in a type
        parent_type = dynamic_cast<const codesh::semantic_analyzer::type_symbol *>(&type_symbol->get());

        // If it isn't, then this supposed "method" cannot exist.
        if (parent_type == nullptr)
        {
            context.blasphemy_consumer(fmt::format(
                codesh::blasphemy::details::TYPE_DOES_NOT_EXIST,
                name.holy_join()
            ), method_call.get_code_position());

            return std::nullopt;
        }
    }


    const auto resolved_method = get_called_method_as_symbol(
        context,
        *parent_type,
        method_call
    );

    if (!resolved_method.has_value())
        return std::nullopt;


    // Update the AST node to the found result
    method_call.set_resolved(resolved_method.value());

    return resolved_method;
}

static bool resolve_arguments(const codesh::semantic_analyzer::semantic_context &context,
                              const codesh::ast::method::operation::method_call_ast_node &method_call_node,
                              const codesh::semantic_analyzer::method_symbol &containing_method,
                              const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    //TODO: When calling non-static methods, also add 'this' as the first argument

    bool all_succeed = true;

    for (const auto &arg : method_call_node.get_arguments())
    {
        if (const auto stmnt = dynamic_cast<codesh::ast::method::operation::method_operation_ast_node *>(arg.get()))
        {
            all_succeed &= codesh::semantic_analyzer::statement::resolve(
                context,
                *stmnt,
                containing_method,
                scope
            );
        }
    }

    return all_succeed;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> get_called_method_as_symbol(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        const codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto method_overloads_raw = type.get_scope().resolve_local(method_call.get_last_name(false));
    if (!method_overloads_raw)
    {
        context.blasphemy_consumer(fmt::format(
            codesh::blasphemy::details::METHOD_NOT_FOUND,
            method_call.to_pretty_string()
        ), method_call.get_code_position());
        return std::nullopt;
    }

    const auto *method_overloads = dynamic_cast<const codesh::semantic_analyzer::method_overloads_symbol *>(&method_overloads_raw->get());
    if (!method_overloads)
    {
        context.blasphemy_consumer(fmt::format(
            codesh::blasphemy::details::NOT_A_METHOD,
            method_call.to_pretty_string()
        ), method_call.get_code_position());
        return std::nullopt;
    }


    // Verify parameter types

    for (const auto &symbol : method_overloads->get_scope().internals() | std::views::values)
    {
        auto &method_symbol = *static_cast<codesh::semantic_analyzer::method_symbol *>(symbol.get()); // NOLINT(*-pro-type-static-cast-downcast)

        const auto &method_params = method_symbol.get_parameter_types();
        const auto &arguments = method_call.get_arguments();

        if (method_params.size() != arguments.size())
            continue;


        // If they're both 0-args long, then they're a perfect match.
        if (method_params.empty())
        {
            return method_symbol;
        }


        for (size_t i = 0; i < method_params.size(); i++)
        {
            const auto method_param_type = method_params.at(i).get();
            const auto argument_value = method_call.get_arguments().at(i).get();

            // Make sure this isn't an error argument
            if (argument_value->get_type() == nullptr)
                return std::nullopt;


            if (codesh::semantic_analyzer::util::are_types_compatible(*argument_value->get_type(), *method_param_type))
            {
                //TODO: Consider "best match", don't just return (casting etc.)
                return method_symbol;
            }
        }
    }

    context.blasphemy_consumer(
        fmt::format(
            codesh::blasphemy::details::ARGUMENT_TYPE_MISMATCH,
            //TODO: Pretty print argument types
            "טודו",
            method_call.to_pretty_string()
        ),
        method_call.get_code_position()
    );
    return std::nullopt;
}
