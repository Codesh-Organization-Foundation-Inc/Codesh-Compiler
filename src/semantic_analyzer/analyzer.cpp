#include "analyzer.h"

#include "../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "passes/check_methods.h"
#include "passes/check_type_declarations.h"
#include "symbol_table/symbol.h"
#include "symbol_table/symbol_table.h"

static void add_default_constructors(const codesh::ast::compilation_unit_ast_node &root_node);
static void add_this_param_to_non_static_methods(const codesh::ast::compilation_unit_ast_node &root_node);
static std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> create_this_param(
        const codesh::ast::type_decl::class_declaration_ast_node &class_decl);

void codesh::semantic_analyzer::run(ast::compilation_unit_ast_node &ast_root)
{
    // Add global scope to symbol table
    //TODO: Resolve all countries of origin
    ast_root.get_symbol_table().add_symbol("", std::make_unique<country_symbol>());

    //TODO: move these to other files
    add_default_constructors(ast_root);
    add_this_param_to_non_static_methods(ast_root);

    add_type_declarations(ast_root);
    check_methods(ast_root);
}

static void add_default_constructors(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const auto class_decl = dynamic_cast<codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_decl)
            continue;

        //TODO: Check if there exists a constructor.
        // Only then should one be added.

        auto constructor_decl = std::make_unique<codesh::ast::constructor_declaration_ast_node>();

        auto attributes_node = std::make_unique<codesh::ast::type_decl::attributes_ast_node>();
        attributes_node->set_visibility(codesh::definition::visibility::PUBLIC);

        constructor_decl->set_attributes(std::move(attributes_node));
        class_decl->get_methods().push_front(std::move(constructor_decl));
    }
}

static void add_this_param_to_non_static_methods(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const auto class_decl = dynamic_cast<codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_decl)
            continue;

        for (const auto &method_decl : class_decl->get_methods())
        {
            if (method_decl->get_attributes()->get_is_static())
                continue;

            method_decl->get_parameters().push_front(
                create_this_param(*class_decl)
            );
        }
    }
}

static std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> create_this_param(
        const codesh::ast::type_decl::class_declaration_ast_node &class_decl)
{
    auto this_param = std::make_unique<codesh::ast::local_variable_declaration_ast_node>();
    this_param->set_name("this");
    this_param->set_is_final(true);

    auto this_class_type = std::make_unique<codesh::ast::type::custom_type_ast_node>();
    this_class_type->set_name(class_decl.get_binary_name());
    this_param->set_type(std::move(this_class_type));

    return std::move(this_param);
}
