#include "resolve.h"

#include "../../../parser/ast/var_reference/variable_reference_ast_node.h"
#include "../../semantic_context.h"
#include "../../symbol_table/symbol.h"
#include "../../symbol_table/symbol_table.h"
#include "fmt/args.h"

/**
 * If the requested name has a single part, will attempt to fetch by the method/class scope.
 * Otherwise, uses imports and such regularly.
 */
static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_symbol_local_first(
        const codesh::semantic_analyzer::semantic_context &context, variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope);


void codesh::semantic_analyzer::statement::variable_reference::resolve(const semantic_context &context,
                                                                       variable_reference_ast_node &var_ref_node,
                                                                       const method_scope_symbol &scope)
{
    const auto result = find_symbol_local_first(context, var_ref_node, scope);
    if (!result.has_value())
        return;

    const auto var_symbol = dynamic_cast<variable_symbol *>(&result.value().get());
    if (var_symbol == nullptr)
    {
        //TODO: Add proper message
        context.blasphemy_consumer(fmt::format(
            "{} אינו משתנה",
            var_ref_node.get_unresolved_name().join(" ל־")
        ));

        return;
    }

    var_ref_node.set_resolved(*var_symbol);
}

static std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol>> find_symbol_local_first(
        const codesh::semantic_analyzer::semantic_context &context, variable_reference_ast_node &var_ref_node,
        const codesh::semantic_analyzer::method_scope_symbol &scope)
{
    const auto &full_var_name = var_ref_node.get_unresolved_name();

    if (!full_var_name.is_single_part())
        return codesh::semantic_analyzer::symbol_table::resolve_from_imports(context, full_var_name);

    // If the variable name is made only with a single part, it MUST be either a local variable, class member,
    //TODO: or static import.

    const auto var_name = full_var_name.get_last_part();

    const auto result = scope.resolve_up(var_name);
    if (!result.has_value())
    {
        // TODO: Add proper message
        context.blasphemy_consumer(fmt::format("{} אינו נמצא", var_name));

        return std::nullopt;
    }

    return *result;
}
