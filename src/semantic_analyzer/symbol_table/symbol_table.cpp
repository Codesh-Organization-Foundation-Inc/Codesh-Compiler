#include "symbol_table.h"

#include "../errors/errors.h"
#include "../../parser/ast/compilation_unit_ast_node.h"

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method_declaration_ast_node &method_decl);


void codesh::semantic_analyzer::symbol_table::collect_types(const ast::compilation_unit_ast_node &root_node,
        country_symbol &containing_country)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const std::string name = type_decl->get_name();

        const auto insert_result = containing_country.add_symbol(
            name, std::make_unique<type_symbol>(
                containing_country,
                type_decl->get_attributes()->get_access_flags()
            )
        );

        if (!insert_result)
        {
            collect_error("Duplicate type declared: " + name);
        }


        if (const auto class_decl = dynamic_cast<const ast::type_decl::class_declaration_ast_node *>(type_decl.get()))
        {
            collect_methods(*class_decl, insert_result.value().get());
        }
    }
}


void codesh::semantic_analyzer::symbol_table::collect_methods(
    const ast::type_decl::class_declaration_ast_node &class_decl, type_symbol &containing_type)
{
    for (const auto &method_decl : class_decl.get_methods())
    {
        const std::string name = method_decl->get_name();

        const auto insert_result = containing_type.add_symbol(
            name, std::make_unique<method_symbol>(
                containing_type,
                method_decl->get_attributes()->get_access_flags(),
                clone_parameter_types(*method_decl),
                method_decl->get_return_type()->clone()
            )
        );

        if (!insert_result)
        {
            collect_error("Duplicate method declared: " + name);
        }

        //TODO: Collect local variables
    }
}

static std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> clone_parameter_types(
        const codesh::ast::method_declaration_ast_node &method_decl)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> result;

    for (const auto &param_node : method_decl.get_parameters())
    {
        result.push_back(param_node->get_type()->clone());
    }

    return result;
}


std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::symbol_table::allowed_symbol_types()
    const
{
    return ALLOWED_SYMBOL_TYPES;
}

codesh::semantic_analyzer::symbol_table::symbol_table(const ast::compilation_unit_ast_node &root_node)
{
    // Add global scope to symbol table
    add_symbol("", std::make_unique<country_symbol>());

    //TODO: Resolve all countries of origin
    auto &global_country = resolve_country("")->get();

    //TODO: Iterate over each and every country, then collect types.
    collect_types(root_node, global_country);
}

const codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::symbol_table::get_symbol_map() const
{
    return global_scope;
}

codesh::semantic_analyzer::named_scope_map &codesh::semantic_analyzer::symbol_table::get_symbol_map()
{
    return global_scope;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::country_symbol>> codesh::semantic_analyzer::
    symbol_table::resolve_country(const std::string &name) const
{
    const auto result = resolve(name);

    if (!result)
        return std::nullopt;

    // Only packages allowed anyway
    return *static_cast<country_symbol *>(&result.value().get()); // NOLINT(*-pro-type-static-cast-downcast)
}

const std::vector<codesh::semantic_analyzer::symbol_type> codesh::semantic_analyzer::symbol_table::ALLOWED_SYMBOL_TYPES = {
    symbol_type::COUNTRY,
    // For the global scope
    symbol_type::TYPE
};
