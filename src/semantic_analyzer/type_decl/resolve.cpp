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

#include <unordered_set>

static void detect_duplicate_interfaces(const codesh::semantic_analyzer::semantic_context &context,
        const std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>> &interface_decls);

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
}

static void detect_duplicate_interfaces(const codesh::semantic_analyzer::semantic_context &context,
        const std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>> &interface_decls)
{
    std::unordered_set<codesh::definition::fully_qualified_name> seen;
    for (const auto &interface : interface_decls)
    {
        if (!interface->is_resolved())
            continue;

        const auto &name = interface->get_resolved().get_full_name();
        if (!seen.insert(name).second)
        {
            context.throw_blasphemy(
                fmt::format(codesh::blasphemy::details::DUPLICATE_INTERFACE, name.holy_join()),
                interface->get_code_position()
            );
        }
    }
}
