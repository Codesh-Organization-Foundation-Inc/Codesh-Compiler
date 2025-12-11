#include "util.h"

#include "../parser/ast/compilation_unit_ast_node.h"

std::optional<std::string> codesh::semantic_analyzer::util::resolve_custom_type(
        const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries, const std::string &name)
{
    if (name == "כתובים")
        return "java/lang/String";

    //TODO: Implement when implementing countries.
    // This method should return the Fully Qualified Class Name of the given type name (which isn't a descriptor).

    return name;
    //TODO: If it is found the the name is an FQCN itself, start the search from the top of the symbol table.
}

bool codesh::semantic_analyzer::util::resolve_custom_type_node(
        const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        ast::type::custom_type_ast_node &symbol_type_node)
{
    const auto resolved_name = resolve_custom_type(lookup_countries, symbol_type_node.get_name());
    if (!resolved_name)
        return false;

    symbol_type_node.set_resolved_name(resolved_name.value());
    return true;
}

bool codesh::semantic_analyzer::util::resolve_custom_type_node(
        const std::vector<std::reference_wrapper<country_symbol>> &lookup_countries,
        ast::type::custom_type_ast_node &custom_type_node,
        ast::type::type_ast_node &symbol_type_node)
{
    const auto resolved_name = resolve_custom_type(lookup_countries, custom_type_node.get_name());
    if (!resolved_name)
        return false;

    custom_type_node.set_resolved_name(resolved_name.value());
    static_cast<ast::type::custom_type_ast_node *>(&symbol_type_node)->set_resolved_name(resolved_name.value()); // NOLINT(*-pro-type-static-cast-downcast)
    return true;
}

codesh::semantic_analyzer::method_overloads_symbol &codesh::semantic_analyzer::util::get_method_overloads_symbol(
    const std::string &name, type_symbol &containing_type)
{
    return containing_type.add_symbol(
        name, std::make_unique<method_overloads_symbol>()
    ).first;
}