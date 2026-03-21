#include "resolve.h"

#include "../../../classpath/loader/interop_replacements.h"
#include "lexer/source_file_info.h"
#include "semantic_analyzer/statement/resolve.h"
#include "semantic_analyzer/statement/variable_reference/resolve.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/method/operation/method_call_ast_node.h"
#include "parser/ast/method/operation/new_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "semantic_analyzer/util.h"
#include "semantic_analyzer/util/poly_util.h"
#include "semantic_analyzer/util/widen_util.h"

#include <ranges>
#include <unordered_set>

struct local_result
{
    codesh::semantic_analyzer::type_symbol *type;
    codesh::semantic_analyzer::variable_symbol *variable;
};

struct parent_type_result
{
    const codesh::semantic_analyzer::type_symbol *parent_type;
    /**
     * Receiver = The variable being passed as `this` to the non-static method
     */
    codesh::semantic_analyzer::variable_symbol *receiver_variable;
};

enum class args_match_type
{
    EXACT_ONLY,
    ALLOW_WIDENING
};

/**
 * Looks up @p name in the current method scope.
 *
 * If found and the symbol is a variable with a resolvable custom type, returns the
 * resolved type alongside the variable symbol.
 *
 * @returns @c std::nullopt if the name is not found, does not refer to a variable,
 * or the variable's type is primitive / not yet resolvable.
 */
static std::optional<local_result> find_custom_type_local_var_by_name(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_scope_symbol &scope,
        const std::string &name);

/**
 * Resolves the parent type of method call from imports, treating the name as a qualified type path
 * with the last segment being the method name.
 * Emits a semantic error and returns @c std::nullopt if the symbol is not found or cannot yield a type.
 */
static std::optional<parent_type_result> resolve_parent_type_from_imports(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::operation::method_call_ast_node &method_call);

/**
 * Determines the parent type and optional receiver variable for a method call.
 * @returns @c std::nullopt if the parent type cannot be resolved.
 */
static std::optional<parent_type_result> resolve_call_parent_type(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        const codesh::ast::method::operation::method_call_ast_node &method_call,
        const codesh::semantic_analyzer::method_scope_symbol &scope);

static std::optional<parent_type_result> resolve_call_parent_type(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::op::new_ast_node &new_call);

static std::optional<parent_type_result> resolve_parent_type_for_chained_call(
        const codesh::ast::method::operation::method_call_ast_node &chained_method);

/**
 * @returns Whether all arguments were successfully resolved.
 */
static bool resolve_arguments(const codesh::semantic_analyzer::semantic_context &context,
                              const codesh::ast::method::operation::method_call_ast_node &method_call_node,
                              const codesh::semantic_analyzer::method_symbol &containing_method,
                              const codesh::semantic_analyzer::method_scope_symbol &scope);

/**
 * For non-static calls on a local variable receiver (e.g. @c obj / @c method), prepends the receiver variable
 * as the first argument.
 */
static void prepend_external_this_argument(
        codesh::ast::method::operation::method_call_ast_node &method_call,
        codesh::semantic_analyzer::variable_symbol &receiver_variable);

/**
 * For non-static calls targeting the same class (e.g. bare @c method or @c this.method), prepends @c this
 * as the first argument.
 *
 * Emits a semantic error if the containing method is static.
 *
 * @returns Whether the operation succeed
 */
static bool prepend_implicit_this_argument(const codesh::semantic_analyzer::semantic_context &context,
                                           codesh::ast::method::operation::method_call_ast_node &method_call,
                                           const codesh::semantic_analyzer::method_symbol &containing_method,
                                           const codesh::semantic_analyzer::method_scope_symbol &scope);

static void maybe_warn_interop_exists(
        const codesh::ast::method::operation::method_call_ast_node &method_call);

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_call(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        codesh::ast::method::operation::method_call_ast_node &method_call,
        const codesh::semantic_analyzer::method_scope_symbol &scope);

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> get_called_method_as_symbol(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        codesh::ast::method::operation::method_call_ast_node &method_call);

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> resolve_method_in_hierarchy(
        const codesh::semantic_analyzer::type_symbol &start,
        const std::string &name);

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_from_overload(
        const codesh::semantic_analyzer::semantic_context &context, args_match_type match_type,
        const codesh::semantic_analyzer::method_overloads_symbol &method_overloads,
        codesh::ast::method::operation::method_call_ast_node &method_call);

static size_t param_offset_of(const codesh::semantic_analyzer::method_symbol &method);

/**
 * @returns A set describing whether an argument at index should widen to allow for a match,
 * or @c std::nullopt if at least one argument cannot be cast to the desired parameters.
 */
static std::optional<std::unordered_set<size_t>> check_args_match(
        const codesh::semantic_analyzer::semantic_context &context, args_match_type match_type,
        const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &params,
        const std::deque<codesh::ast::method::operation::named_argument> &arguments, size_t offset);



bool codesh::semantic_analyzer::statement::method_call::resolve(const semantic_context &context,
                                                     ast::method::operation::method_call_ast_node &method_call,
                                                     const method_symbol &containing_method,
                                                     const method_scope_symbol &scope)
{
    const auto result = resolve_method_call(
        context,
        containing_method,
        method_call,
        scope
    );

    if (!result.has_value())
        return false;

    return true;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_call(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        codesh::ast::method::operation::method_call_ast_node &method_call,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    // We must first resolve the method's arguments
    // if we want to determine which argument types we pass forward (overloading)
    if (!resolve_arguments(context, method_call, containing_method, scope))
        return std::nullopt;

    // Verify that there aren't any error types (error_value_ast_node)
    // The error they cause during semantic analysis is that their type is null,
    // so check that instead of dynamic_cast:
    const auto &arguments = method_call.get_arguments();

    for (const auto &arg : arguments)
    {
        if (arg.value->get_type() == nullptr)
            return std::nullopt;
    }

    // Recursively resolve all chained methods first
    if (method_call.has_chained_method())
    {
        resolve_method_call(
            context,
            containing_method,
            method_call.get_chained_method(),
            scope
        );
    }

    const auto &name = method_call.get_unresolved_name();
    if (name.get_parts().empty())
    {
        // Error in parsing phase; Ignore
        return std::nullopt;
    }

    const auto result = resolve_call_parent_type(context, containing_method, method_call, scope);
    if (!result.has_value())
        return std::nullopt;

    const auto [parent_type, receiver_variable] = *result;


    const auto resolved_method = get_called_method_as_symbol(
        context,
        *parent_type,
        method_call
    );

    if (!resolved_method.has_value())
        return std::nullopt;


    // Update the AST node to the found result
    method_call.set_resolved(resolved_method.value());

    if (resolved_method->get().get_producing_node() == nullptr)
    {
        maybe_warn_interop_exists(method_call);
    }

    // Handle prepending `this` for non-static method calls
    if (!resolved_method->get().get_attributes().get_is_static())
    {
        // For calls on local variables, prepend the receiver:
        if (receiver_variable != nullptr)
        {
            prepend_external_this_argument(method_call, *receiver_variable);
        }
        else
        {
            if (!prepend_implicit_this_argument(context, method_call, containing_method, scope))
                return std::nullopt;
        }
    }


    // For new calls, also resolve the constructed type:
    if (const auto new_call = dynamic_cast<const codesh::ast::op::new_ast_node *>(&method_call))
    {
        codesh::semantic_analyzer::util::resolve_type_node(
            context,
            new_call->get_constructed_type()
        );
    }

    return resolved_method;
}

static std::optional<parent_type_result> resolve_call_parent_type_for_super(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        const codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto &current_type = containing_method.get_parent_type();
    if (!current_type.has_super_type() || !current_type.get_super_type().is_resolved())
    {
        context.throw_blasphemy(
            fmt::format(codesh::blasphemy::details::TYPE_DOES_NOT_EXIST, "super"),
            method_call.get_name_range()
        );
        return std::nullopt;
    }

    return parent_type_result {
        &current_type.get_super_type().get_resolved(),
        nullptr
    };
}

static std::optional<parent_type_result> resolve_call_parent_type(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_symbol &containing_method,
        const codesh::ast::method::operation::method_call_ast_node &method_call,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    const auto &name = method_call.get_unresolved_name();

    if (const auto *new_call = dynamic_cast<const codesh::ast::op::new_ast_node *>(&method_call))
        return resolve_call_parent_type(context, *new_call);

    if (method_call.get_association() == codesh::ast::var_reference::reference_association::SUPER)
        return resolve_call_parent_type_for_super(context, containing_method, method_call);

    // For "this" methods, we must be speaking of type members.
    // For single-names, the method must either be a type member or a static import.
    //TODO: Handle static imports
    if (method_call.get_association() == codesh::ast::var_reference::reference_association::THIS
        || name.is_single_part())
    {
        return parent_type_result {
            &containing_method.get_parent_type(),
            nullptr
        };
    }

    if (method_call.has_chained_method())
    {
        return resolve_parent_type_for_chained_call(method_call.get_chained_method());
    }

    // Check if the front of the name matches a local variable in the scope.
    // ויעש משתנה ל־מעשה...
    const auto &front = method_call.get_unresolved_name().get_parts().front();

    if (const auto result = find_custom_type_local_var_by_name(context, scope, front))
    {
        return parent_type_result {
            result->type,
            result->variable
        };
    }

    return resolve_parent_type_from_imports(context, method_call);
}

static std::optional<parent_type_result> resolve_call_parent_type(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::op::new_ast_node &new_call)
{
    // For new calls, the parent type is the constructed type, not the containing class.
    const auto resolved_type = codesh::semantic_analyzer::util::resolve_custom_type(
        context,
        new_call.get_constructed_type()
    );

    if (!resolved_type.has_value())
        return std::nullopt;

    return parent_type_result {
        &resolved_type->get(),
        nullptr
    };
}

static std::optional<parent_type_result> resolve_parent_type_for_chained_call(
        const codesh::ast::method::operation::method_call_ast_node &chained_method)
{
    if (!chained_method.is_resolved())
        return std::nullopt;

    // For chained methods, `this` is their return value
    const auto *return_type = dynamic_cast<const codesh::ast::type::custom_type_ast_node *>(
        &chained_method.get_resolved().get_return_type()
    );

    if (return_type == nullptr)
    {
        // Non-custom return type - can't chain further calls on it.
        // E.g. a primitive
        return std::nullopt;
    }

    return parent_type_result {
        &return_type->get_resolved(),
        nullptr
    };
}

static std::optional<local_result> find_custom_type_local_var_by_name(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::method_scope_symbol &scope,
        const std::string &name)
{
    const auto local_res = scope.resolve_up(name);
    if (!local_res.has_value())
        return std::nullopt;

    auto *var_sym = dynamic_cast<codesh::semantic_analyzer::variable_symbol *>(&local_res->get());
    if (var_sym == nullptr)
        return std::nullopt;

    const auto resolved_type = codesh::semantic_analyzer::util::resolve_custom_type_node(
        context,
        *var_sym->get_type()
    );
    if (!resolved_type.has_value())
        return std::nullopt;

    return local_result {
        &resolved_type->get(),
        var_sym
    };
}

static void prepend_external_this_argument(
        codesh::ast::method::operation::method_call_ast_node &method_call,
        codesh::semantic_analyzer::variable_symbol &receiver_variable)
{
    auto receiver_node = std::make_unique<codesh::ast::var_reference::variable_reference_ast_node>(
        method_call.get_code_position(),
        codesh::definition::fully_qualified_name(
            method_call.get_code_position(),
            method_call.get_unresolved_name().get_parts().front()
        )
    );

    receiver_node->set_resolved(receiver_variable);

    method_call.get_arguments().emplace_front("this", std::move(receiver_node));
}

static bool prepend_implicit_this_argument(const codesh::semantic_analyzer::semantic_context &context,
                                           codesh::ast::method::operation::method_call_ast_node &method_call,
                                           const codesh::semantic_analyzer::method_symbol &containing_method,
                                           const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    // Method calls get the result and do not need `this`
    if (method_call.has_chained_method())
        return true;

    // New calls don't need `this`
    if (dynamic_cast<const codesh::ast::op::new_ast_node *>(&method_call))
        return true;


    if (containing_method.get_attributes().get_is_static())
    {
        context.throw_blasphemy(
            fmt::format(
                codesh::blasphemy::details::NON_STATIC_CALL_FROM_STATIC_CONTEXT,
                method_call.get_last_name(false)
            ),
            {
                method_call.get_unresolved_name().get_source_range().start,
                method_call.get_name_range().end
            }
        );
        return false;
    }

    // 'this' is always in scope as a local_variable_symbol at slot 0 (added by prepare())
    const auto this_symbol = scope.resolve_up("this");
    auto &this_var_symbol = static_cast<codesh::semantic_analyzer::variable_symbol &>(this_symbol->get()); // NOLINT(*-pro-type-static-cast-downcast)

    auto this_var = std::make_unique<codesh::ast::var_reference::variable_reference_ast_node>(
        method_call.get_code_position(),
        codesh::definition::fully_qualified_name(method_call.get_code_position(), "this")
    );

    this_var->set_resolved(this_var_symbol);

    method_call.get_arguments().emplace_front("this", std::move(this_var));

    return true;
}

static bool resolve_arguments(const codesh::semantic_analyzer::semantic_context &context,
                              const codesh::ast::method::operation::method_call_ast_node &method_call_node,
                              const codesh::semantic_analyzer::method_symbol &containing_method,
                              const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    bool all_succeed = true;

    for (const auto &[arg_name, arg_value] : method_call_node.get_arguments())
    {
        if (const auto stmnt = dynamic_cast<codesh::ast::method::operation::method_operation_ast_node *>(arg_value.get()))
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

static std::optional<parent_type_result> resolve_parent_type_from_imports(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto &name = method_call.get_unresolved_name();

    const auto resolved = context.symbol_table_.resolve(
        context,
        name,
        // Ignore the last part of the name, which points to the method overloads.
        // get_called_method_as_symbol already handles it.
        name.get_parts().end() - 1
    );

    if (!resolved.has_value())
        return std::nullopt;

    // There are a few possible methods a method can be called from:
    // - Directly from the type
    if (const auto *parent_type = dynamic_cast<const codesh::semantic_analyzer::type_symbol *>(&resolved->get()))
    {
        return parent_type_result {
            parent_type,
            nullptr
        };
    }

    // - Via a variable/field (e.g. System.out.println)
    if (auto *field = dynamic_cast<codesh::semantic_analyzer::variable_symbol *>(&resolved->get()))
    {
        // Field type must be custom for it to have contents (e.g System.out.println, out is PrintStream)
        const auto field_type = codesh::semantic_analyzer::util::resolve_custom_type_node(
            context,
            *field->get_type()
        );

        if (field_type.has_value())
        {
            return parent_type_result {
                &field_type->get(),
                field
            };
        }
    }

    // If it's neither a type nor a field, then this supposed "method" cannot exist.
    context.throw_blasphemy(fmt::format(
        codesh::blasphemy::details::TYPE_DOES_NOT_EXIST,
        name.holy_join()
    ), method_call.get_name_range());

    return std::nullopt;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> get_called_method_as_symbol(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto method_overloads_raw = resolve_method_in_hierarchy(
        type,
        method_call.get_last_name(false)
    );

    if (!method_overloads_raw.has_value())
    {
        context.throw_blasphemy(fmt::format(
            codesh::blasphemy::details::METHOD_NOT_FOUND,
            method_call.to_pretty_string()
        ), method_call.get_name_range());
        return std::nullopt;
    }

    const auto *method_overloads = dynamic_cast<const codesh::semantic_analyzer::method_overloads_symbol *>(
        &method_overloads_raw->get()
    );

    if (!method_overloads)
    {
        context.throw_blasphemy(fmt::format(
            codesh::blasphemy::details::NOT_A_METHOD,
            method_call.to_pretty_string()
        ), method_call.get_name_range());
        return std::nullopt;
    }


    const auto exact_method = resolve_method_from_overload(
        context,
        args_match_type::EXACT_ONLY,
        *method_overloads,
        method_call
    );

    if (exact_method.has_value())
        return exact_method;

    const auto ma_zot_omeret_beereh_method = resolve_method_from_overload(
        context,
        args_match_type::ALLOW_WIDENING,
        *method_overloads,
        method_call
    );

    if (ma_zot_omeret_beereh_method.has_value())
        return ma_zot_omeret_beereh_method;


    context.throw_blasphemy(
        fmt::format(
            codesh::blasphemy::details::ARGUMENT_TYPE_MISMATCH,
            //TODO: Pretty print argument types
            "טודו",
            method_call.to_pretty_string()
        ),
        //TODO: Highlight arguments, not method name
        method_call.get_name_range()
    );
    return std::nullopt;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> resolve_method_in_hierarchy(
        const codesh::semantic_analyzer::type_symbol &start,
        const std::string &name)
{
    const codesh::semantic_analyzer::type_symbol *current = &start;
    while (current != nullptr)
    {
        const auto result = current->get_scope().resolve_local(name);
        if (result.has_value())
            return result;

        if (!current->has_super_type() || !current->get_super_type().is_resolved())
            break;

        current = &current->get_super_type().get_resolved();
    }

    return std::nullopt;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::method_symbol>> resolve_method_from_overload(
        const codesh::semantic_analyzer::semantic_context &context, const args_match_type match_type,
        const codesh::semantic_analyzer::method_overloads_symbol &method_overloads,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    auto &arguments = method_call.get_arguments();

    for (const auto &symbol : method_overloads.get_scope().internals() | std::views::values)
    {
        auto &method = *static_cast<codesh::semantic_analyzer::method_symbol *>(symbol.get()); // NOLINT(*-pro-type-static-cast-downcast)

        const size_t offset = param_offset_of(method);
        const auto &params  = method.get_parameter_types();

        if (params.size() - offset != arguments.size())
            continue;

        // At this point, it is bound that the method arguments are the same size.
        // `arguments` does not include an implicit `this`.
        //
        // So if the arguments' size is 0, it means that this method does not get any parameters;
        // No arguments means an early exact match.
        if (arguments.empty())
            return method;

        const auto args_match_result = check_args_match(
            context,
            match_type,
            params,
            arguments,
            offset
        );
        if (!args_match_result.has_value())
            continue;

        const auto &needs_widening = args_match_result.value();

        // Wrap each argument that needs widening
        for (size_t i = 0; i < arguments.size(); i++)
        {
            if (needs_widening.contains(i))
            {
                arguments.at(i).value = codesh::semantic_analyzer::util::make_widening_cast(
                    std::move(arguments.at(i).value),
                    *params.at(i + offset)
                );
            }
        }

        return method;
    }

    return std::nullopt;
}

static std::optional<std::unordered_set<size_t>> check_args_match(
        const codesh::semantic_analyzer::semantic_context &context,
        const args_match_type match_type,
        const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &params,
        const std::deque<codesh::ast::method::operation::named_argument> &arguments, const size_t offset)
{
    std::unordered_set<size_t> match_results;
    match_results.reserve(arguments.size());

    for (size_t i = 0; i < arguments.size(); i++)
    {
        auto &param_type = *params.at(i + offset);
        const auto &arg_type = *arguments.at(i).value->get_type();

        if (!codesh::semantic_analyzer::util::resolve_type_node(context, param_type))
            return std::nullopt;


        const bool is_exact_type = codesh::semantic_analyzer::util::do_types_match(arg_type, param_type)
            || codesh::semantic_analyzer::util::can_poly_cast_to(arg_type, param_type);

        if (!is_exact_type)
        {
            if (!codesh::semantic_analyzer::util::can_widen_to(arg_type, param_type))
                return std::nullopt;

            if (match_type == args_match_type::ALLOW_WIDENING)
                match_results.emplace(i);
            else
                return std::nullopt;
        }
    }

    return match_results;
}

static size_t param_offset_of(const codesh::semantic_analyzer::method_symbol &method)
{
    const bool is_external = method.get_producing_node() == nullptr;

    return !is_external && !method.get_attributes().get_is_static()
        ? 1
        : 0;
}

static void maybe_warn_interop_exists(
        const codesh::ast::method::operation::method_call_ast_node &method_call)
{
    const auto replacement = codesh::semantic_analyzer::external::find_codesh_replacement(
        method_call.get_unresolved_name()
    );

    if (!replacement.has_value())
        return;

    codesh::blasphemy::get_blasphemy_collector().add_warning(
        fmt::format(
            codesh::blasphemy::details::INTEROP_REPLACEMENT_EXISTS,
            method_call.get_unresolved_name().holy_join(), replacement->holy_join()
        ),
        codesh::blasphemy::blasphemy_type::SEMANTIC,
        method_call.get_unresolved_name().get_source_range()
    );
}
