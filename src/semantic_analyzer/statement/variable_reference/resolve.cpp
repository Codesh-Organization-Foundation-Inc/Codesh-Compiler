#include "resolve.h"

#include "blasphemy/details.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

/**
 * If the requested name has a single part, will attempt to fetch by the method/class scope.
 * Otherwise, uses imports and such regularly.
 */
static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_symbol_local_first(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope);

static bool resolve_variable_reference(const codesh::semantic_analyzer::semantic_context &context,
        codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope);

/**
 * @returns Whether that the local variable referenced is accessed when it's supposed to
 */
static bool is_accessible(const codesh::semantic_analyzer::local_variable_symbol &local_var,
                          const codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
                          const codesh::semantic_analyzer::method_scope_symbol &scope);

/**
 * Resolves a field reference of the form @c this.fieldName by looking up the field name
 * directly in the enclosing type's scope.
 */
static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_field_symbol(
        const codesh::semantic_analyzer::semantic_context &context, const codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope);


bool codesh::semantic_analyzer::statement::variable_reference::resolve(const semantic_context &context,
        ast::var_reference::variable_reference_ast_node &var_ref_node,
        const method_scope_symbol &scope)
{
    if (!resolve_variable_reference(context, var_ref_node, scope))
        return false;

    //TODO: Add type checks

    if (const auto &local_var = dynamic_cast<const local_variable_symbol *>(&var_ref_node.get_resolved()))
    {
        if (!is_accessible(*local_var, var_ref_node, scope))
        {
            const auto &local_var_node = local_var->get_producing_node();

            // TODO: Proper message
            context.throw_blasphemy(
                fmt::format(
                    blasphemy::details::VARIABLE_REFERENCED_BEFORE_CREATION,
                    local_var_node->get_name()
                ),
                var_ref_node.get_unresolved_name().get_source_range()
            );
        }
    }

    return true;
}

static bool is_accessible(const codesh::semantic_analyzer::local_variable_symbol &local_var,
                          const codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
                          const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    // Only validate variable range if the reference is within the SAME scope as the declaration.
    // References from nested scopes to outer-scope variables are always valid.
    //
    // To get the local_var parameter we use resolve_up anyway,
    // so we literally can't get a variable from a scope lower than the current.
    if (local_var.get_parent_symbol() != static_cast<const codesh::semantic_analyzer::i_scope_containing_symbol *>(
        &scope
    ))
    {
        return true;
    }


    const auto &local_var_node = local_var.get_producing_node();

    const size_t min = local_var_node->get_accessible_from();
    const size_t max = local_var_node->get_accessible_to();
    const size_t curr = var_ref_node.get_statement_index();

    return min <= curr && curr <= max;
}

static bool resolve_variable_reference(const codesh::semantic_analyzer::semantic_context &context,
        codesh::ast::var_reference::variable_reference_ast_node &var_ref_node, const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    if (var_ref_node.get_producing_declaration().has_value())
    {
        // This node was produced as a result of a variable declaration.
        // It is easily resolvable to the same symbol of the declaration itself.
        var_ref_node.set_resolved(var_ref_node.get_producing_declaration()->get().get_resolved());
        return true;
    }


    const auto result = find_symbol_local_first(context, var_ref_node, scope);
    if (!result.has_value())
        return false;

    const auto var_symbol = dynamic_cast<codesh::semantic_analyzer::variable_symbol *>(&result.value().get());
    if (var_symbol == nullptr)
    {
        context.throw_blasphemy(fmt::format(
            codesh::blasphemy::details::NOT_A_VARIABLE,
            var_ref_node.get_unresolved_name().holy_join()
        ), var_ref_node.get_unresolved_name().get_source_range());

        return false;
    }

    var_ref_node.set_resolved(*var_symbol);
    return true;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_symbol_local_first(
        const codesh::semantic_analyzer::semantic_context &context,
        const codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    const auto &full_var_name = var_ref_node.get_unresolved_name();

    if (!full_var_name.is_single_part())
    {
        if (var_ref_node.get_association() == codesh::ast::var_reference::reference_association::THIS)
            return find_field_symbol(context, var_ref_node, scope);

        return context.symbol_table_.resolve(
            context,
            full_var_name
        );
    }


    // If the variable name is made only with a single part, it MUST be either a local variable, class member,
    //TODO: or static import.

    const auto var_name = full_var_name.get_last_part();

    const auto result = scope.resolve_up(var_name);
    if (!result.has_value())
    {
        context.throw_blasphemy(fmt::format(
            codesh::blasphemy::details::SYMBOL_NOT_FOUND,
            var_name
        ), var_ref_node.get_unresolved_name().get_source_range());

        return std::nullopt;
    }

    return *result;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_field_symbol(
        const codesh::semantic_analyzer::semantic_context &context, const codesh::ast::var_reference::variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    const auto &type = scope.get_producing_node()
        ->get_parent_method()
        .get_resolved()
        .get_parent_type();

    const auto field_name = var_ref_node.get_unresolved_name().get_last_part();
    const auto result = type.get_field_scope().resolve_local(field_name);

    if (!result.has_value())
    {
        context.throw_blasphemy(fmt::format(
            codesh::blasphemy::details::SYMBOL_NOT_FOUND,
            field_name
        ), var_ref_node.get_unresolved_name().get_source_range());
    }

    return result;
}
