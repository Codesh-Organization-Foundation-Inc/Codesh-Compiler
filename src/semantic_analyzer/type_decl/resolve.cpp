#include "resolve.h"
#include "semantic_analyzer/field_decl/resolve.h"
#include "semantic_analyzer/method_decl/resolve.h"

#include "fmt/xchar.h"
#include "parser/ast/compilation_unit_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type_declaration/error_type_declaration_ast_node.h"
#include "blasphemy/details.h"
#include "semantic_analyzer/semantic_context.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/util.h"

#include <ranges>
#include <unordered_map>
#include <unordered_set>

/**
 * Method name -> param descriptors
 */
using abstract_method_map = std::unordered_map<std::string, std::unordered_set<std::string>>;

static void detect_duplicate_interfaces(const codesh::semantic_analyzer::semantic_context &context,
        const std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>> &interface_decls);

static void check_unimplemented_methods(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        const codesh::ast::type_decl::type_declaration_ast_node &type_decl);
static bool is_method_implemented_in_hierarchy(const codesh::semantic_analyzer::type_symbol &type,
        const std::string &method_name, const std::string &params_descriptor);
static void collect_abstract_methods(const codesh::semantic_analyzer::type_symbol &interface,
        abstract_method_map &out);

void codesh::semantic_analyzer::type_declaration::resolve(const semantic_context &context,
                                                          const ast::type_decl::type_declaration_ast_node &type_decl,
                                                          const country_symbol &country)
{
    // No reason to resolve an error node
    if (dynamic_cast<const ast::type_decl::error_type_declaration_ast_node *>(&type_decl))
        return;

    const auto name = type_decl.get_last_name(false);
    const auto new_context = context.with_consumer("בָּעֶצֶם", name);

    const auto &type = *static_cast<type_symbol *>(&country.get_scope().resolve_local(name).value().get()); // NOLINT(*-pro-type-static-cast-downcast)


    // Super class
    util::resolve_type_node(
        context,
        *type_decl.get_super_class(),
        type.get_super_type()
    );

    // Interfaces
    const auto &interface_syms = type.get_interfaces();
    const auto &interface_decls = type_decl.get_interfaces();
    for (size_t i = 0; i < interface_decls.size(); i++)
    {
        util::resolve_type_node(context, *interface_decls[i], *interface_syms[i]);
    }

    detect_duplicate_interfaces(context, interface_decls);


    for (const auto &field_decl : type_decl.get_fields())
    {
        field_declaration::resolve(new_context, type, *field_decl);
    }
    for (const auto &method_decl : type_decl.get_all_methods())
    {
        method_declaration::resolve(new_context, type, *method_decl);
    }

    check_unimplemented_methods(context, type, type_decl);
}

static void detect_duplicate_interfaces(const codesh::semantic_analyzer::semantic_context &context,
        const std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>> &interface_decls)
{
    std::unordered_set<std::string> seen;
    for (const auto &interface : interface_decls)
    {
        if (!interface->is_resolved())
            continue;

        const auto &fqn = interface->get_resolved().get_full_name();
        if (!seen.insert(fqn.join()).second)
        {
            context.throw_blasphemy(
                fmt::format(codesh::blasphemy::details::DUPLICATE_INTERFACE, fqn.holy_join()),
                interface->get_code_position()
            );
        }
    }
}

static void check_unimplemented_methods(const codesh::semantic_analyzer::semantic_context &context,
        const codesh::semantic_analyzer::type_symbol &type,
        const codesh::ast::type_decl::type_declaration_ast_node &type_decl)
{
    for (const auto &interface_node : type.get_interfaces())
    {
        if (!interface_node->is_resolved())
            continue;

        abstract_method_map abstract_methods;
        collect_abstract_methods(interface_node->get_resolved(), abstract_methods);

        for (const auto &[method_name, required_params_set] : abstract_methods)
        {
            for (const auto &required_params : required_params_set)
            {
                if (!is_method_implemented_in_hierarchy(type, method_name, required_params))
                {
                    context.throw_blasphemy(
                        fmt::format(codesh::blasphemy::details::UNIMPLEMENTED_METHOD,
                            type.get_full_name().holy_join(),
                            "טודו", //TODO: Full method signature
                            interface_node->get_resolved().get_full_name().holy_join()
                        ),
                        type_decl.get_code_position()
                    );
                }
            }
        }
    }
}

static void collect_abstract_methods(const codesh::semantic_analyzer::type_symbol &interface,
        abstract_method_map &out)
{
    for (const auto &[method_name, overloads_raw] : interface.get_scope().internals())
    {
        const auto &overloads = static_cast<const codesh::semantic_analyzer::method_overloads_symbol &>(*overloads_raw); // NOLINT(*-pro-type-static-cast-downcast)
        for (const auto &[params_descriptor, method_raw] : overloads.get_scope().internals())
        {
            const auto &method = static_cast<const codesh::semantic_analyzer::method_symbol &>(*method_raw); // NOLINT(*-pro-type-static-cast-downcast)

            if (method.get_attributes().get_is_abstract())
            {
                out[method_name].insert(params_descriptor);
            }
        }
    }

    if (interface.has_super_type() && interface.get_super_type().is_resolved())
    {
        collect_abstract_methods(interface.get_super_type().get_resolved(), out);
    }

    for (const auto &parent_interface : interface.get_interfaces())
    {
        if (parent_interface->is_resolved())
        {
            collect_abstract_methods(parent_interface->get_resolved(), out);
        }
    }
}

static bool is_method_implemented_in_hierarchy(const codesh::semantic_analyzer::type_symbol &type,
        const std::string &method_name, const std::string &params_descriptor)
{
    if (const auto overloads_raw = type.get_scope().resolve_local(method_name))
    {
        const auto &overloads = static_cast<const codesh::semantic_analyzer::method_overloads_symbol &>( // NOLINT(*-pro-type-static-cast-downcast)
            overloads_raw->get()
        );

        if (overloads.resolve_method(params_descriptor).has_value())
            return true;
    }

    if (type.has_super_type() && type.get_super_type().is_resolved())
    {
        return is_method_implemented_in_hierarchy(
            type.get_super_type().get_resolved(),
            method_name,
            params_descriptor
        );
    }

    return false;
}
