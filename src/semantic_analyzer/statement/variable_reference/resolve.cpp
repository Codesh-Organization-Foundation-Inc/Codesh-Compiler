#include "resolve.h"

#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "blasphemy/details.h"
#include "fmt/args.h"

/**
 * If the requested name has a single part, will attempt to fetch by the method/class scope.
 * Otherwise, uses imports and such regularly.
 */
static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_symbol_local_first(
        const codesh::semantic_analyzer::semantic_context &context, variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope);

static bool resolve_variable_reference(const codesh::semantic_analyzer::semantic_context &context,
        variable_reference_ast_node &var_ref_node, const codesh::semantic_analyzer::method_scope_symbol &scope);


bool codesh::semantic_analyzer::statement::variable_reference::resolve(const semantic_context &context,
                                                                       variable_reference_ast_node &var_ref_node,
                                                                       const method_scope_symbol &scope)
{
    if (!resolve_variable_reference(context, var_ref_node, scope))
        return false;

    //TODO: Add type checks

    if (const auto &local_var = dynamic_cast<const local_variable_symbol *>(&var_ref_node.get_resolved()))
    {
        // Validate that the local variable referenced is accessed when it's supposed to
        const auto &local_var_node = local_var->get_producing_node();

        const size_t min = local_var_node->get_accessible_from();
        const size_t max = local_var_node->get_accessible_to();
        const size_t curr = var_ref_node.get_statement_index();

        if (curr > max || curr < min)
        {
            //TODO: Proper message
            context.blasphemy_consumer(fmt::format(
                blasphemy::details::VARIABLE_REFERENCED_BEFORE_CREATION,
                local_var_node->get_name()
            ), var_ref_node.get_code_position());
        }
    }

    return true;
}

static bool resolve_variable_reference(const codesh::semantic_analyzer::semantic_context &context,
        variable_reference_ast_node &var_ref_node, const codesh::semantic_analyzer::method_scope_symbol &scope)
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
        context.blasphemy_consumer(fmt::format(
            codesh::blasphemy::details::NOT_A_VARIABLE,
            var_ref_node.get_unresolved_name().holy_join()
        ), var_ref_node.get_code_position());

        return false;
    }

    var_ref_node.set_resolved(*var_symbol);
    return true;
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_symbol_local_first(
        const codesh::semantic_analyzer::semantic_context &context, variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    const auto &full_var_name = var_ref_node.get_unresolved_name();

    if (!full_var_name.is_single_part())
    {
        return codesh::semantic_analyzer::symbol_table::resolve_from_imports(
            context,
            full_var_name,
            var_ref_node.get_code_position()
        );
    }


    // If the variable name is made only with a single part, it MUST be either a local variable, class member,
    //TODO: or static import.

    const auto var_name = full_var_name.get_last_part();

    const auto result = scope.resolve_up(var_name);
    if (!result.has_value())
    {
        context.blasphemy_consumer(fmt::format(
            codesh::blasphemy::details::SYMBOL_NOT_FOUND,
            var_name
        ), var_ref_node.get_code_position());

        return std::nullopt;
    }

    return *result;
}
