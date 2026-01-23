#include "analyzer.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/blasphemy_consumer.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/constructor_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/method/operation/return_ast_node.h"
#include "parser/ast/method/operation/super_call_ast_node.h"
#include "parser/ast/method/method_scope_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "semantic_analyzer/statement/resolve.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/type_decl/collect.h"
#include "semantic_analyzer/type_decl/resolve.h"
#include "semantic_analyzer/type_decl/resolve_aliases.h"
#include "semantic_context.h"
#include "symbol_table/symbol_table.h"

/**
 * When found that a class does not extend anything, will automatically extend `java/lang/Object`.
 */
static void add_default_super_class(const codesh::ast::compilation_unit_ast_node &root_node);
static void add_default_constructor(const codesh::ast::compilation_unit_ast_node &root_node);
/**
 * When found that a constructor does not call super, will automatically call it with no arguments.
 */
static void add_default_super_call(const codesh::ast::compilation_unit_ast_node &root_node);
/**
 * When found that a method returning void does not call return in one of its paths,
 * will automatically add it.
 */
static void add_default_return_statement(const codesh::ast::compilation_unit_ast_node &root_node);

static void add_this_param_to_non_static_methods(const codesh::ast::compilation_unit_ast_node &root_node);
static std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> create_this_param(
        const codesh::ast::type_decl::class_declaration_ast_node &class_decl);

static void resolve_method_bodies(const codesh::semantic_analyzer::semantic_context &context);


const codesh::definition::fully_qualified_name codesh::semantic_analyzer::DEFAULT_SUPER_CLASS_NAME = "java/lang/Object";


void codesh::semantic_analyzer::prepare(const ast::compilation_unit_ast_node &ast_root)
{
    add_default_super_class(ast_root);

    add_default_constructor(ast_root);
    add_default_super_call(ast_root);

    add_default_return_statement(ast_root);

    add_this_param_to_non_static_methods(ast_root);
}

void codesh::semantic_analyzer::collect_symbols(const ast::compilation_unit_ast_node &ast_root,
                                                const symbol_table &table)
{
    //TODO: Use actual countries
    const std::vector lookup_countries = {
        table.resolve_country("").value()
    };
    country_symbol &country = lookup_countries.back();

    const semantic_context context = {lookup_countries, ast_root, blasphemy::semantic_consumer};

    type_declaration::collect_types(context, country);
}

void codesh::semantic_analyzer::analyze(const ast::compilation_unit_ast_node &ast_root,
                                        const symbol_table &table)
{
    //TODO: Use actual countries
    const std::vector lookup_countries = {
        table.resolve_country("").value()
    };
    country_symbol &country = lookup_countries.back();

    const semantic_context context = {lookup_countries, ast_root, blasphemy::semantic_consumer};

    type_declaration::collect_methods(context, country);
    type_declaration::resolve(context, country);

    // Only after collecting all types should we resolve all the methods' bodies:
    resolve_method_bodies(context);

    type_declaration::resolve_aliases(context, country);
}


static void resolve_method_bodies(const codesh::semantic_analyzer::semantic_context &context)
{
    // It's long because we need to get veeery deep up until we reach each method:
    for (const auto &type_decl : context.root.get_type_declarations())
    {
        const auto type_context = context.with_consumer("בָּעֶצֶם", type_decl->get_last_name(false));

        for (const auto &method_decl : type_decl->get_all_methods())
        {
            const auto method_context = context.with_consumer("בְּמַעֲשֶׂה", method_decl->get_last_name(false));

            //TODO: Handle multiple scopes
            const codesh::ast::method::method_scope_ast_node &method_scope = method_decl->get_method_scope();

            for (const auto &stmnt : method_scope.get_body())
            {
                codesh::semantic_analyzer::statement::resolve(
                    method_context,
                    *stmnt,
                    method_decl->get_resolved(),
                    method_scope.get_resolved()
                );
            }
        }
    }
}


// Prepares

static void add_default_super_class(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        if (type_decl->get_super_class() != nullptr)
            continue;


        type_decl->set_super_class(std::make_unique<codesh::ast::type::custom_type_ast_node>(
            codesh::semantic_analyzer::DEFAULT_SUPER_CLASS_NAME
        ));
    }
}

static void add_default_constructor(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const auto class_decl = dynamic_cast<codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_decl)
            continue;

        // Only add a default constructor if one does not exist
        if (!class_decl->get_constructors().empty())
            continue;

        auto constructor_decl = std::make_unique<codesh::ast::method::constructor_declaration_ast_node>();

        auto attributes_node = std::make_unique<codesh::ast::type_decl::attributes_ast_node>();
        attributes_node->set_visibility(codesh::definition::visibility::PUBLIC);
        constructor_decl->set_attributes(std::move(attributes_node));

        class_decl->add_method(std::move(constructor_decl));
    }
}

static void add_default_super_call(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const auto class_decl = dynamic_cast<codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_decl)
            continue;

        for (const auto constructor : class_decl->get_constructors())
        {
            auto &scope = constructor->get_method_scope();
            const auto &body = scope.get_body();

            // Only add super calls to those who lack it
            //TODO: Also filter "this" calls
            if (!body.empty() && dynamic_cast<const codesh::ast::method::operation::super_call_ast_node *>(body.front().get()))
                continue;

            scope.push_front_statement(
                std::make_unique<codesh::ast::method::operation::super_call_ast_node>()
            );
        }
    }
}

static void add_default_return_statement(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {

        for (const auto &method : type_decl->get_all_methods())
        {
            // This is only relevant for void-returning methods
            const codesh::ast::type::primitive_type_ast_node *return_type =
                dynamic_cast<const codesh::ast::type::primitive_type_ast_node *>(method->get_return_type());

            if (return_type == nullptr)
                continue;
            if (return_type->get_type() != codesh::definition::primitive_type::VOID)
                continue;


            auto &scope = method->get_method_scope();
            const auto &body = scope.get_body();

            //TODO: This SHOULD check all PATHS, not the last line alone.
            if (!body.empty() && dynamic_cast<const codesh::ast::method::operation::return_ast_node *>(body.front().get()))
                continue;

            scope.add_statement(
                std::make_unique<codesh::ast::method::operation::return_ast_node>()
            );
        }
    }
}


static void add_this_param_to_non_static_methods(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const auto class_decl = dynamic_cast<codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_decl)
            continue;

        for (const auto &method_decl : class_decl->get_all_methods())
        {
            if (method_decl->get_attributes()->get_is_static())
                continue;

            method_decl->add_parameter(
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

    auto attributes_node = std::make_unique<codesh::ast::type_decl::attributes_ast_node>();
    attributes_node->set_is_final(true);
    this_param->set_attributes(std::move(attributes_node));

    auto this_class_type = std::make_unique<codesh::ast::type::custom_type_ast_node>(class_decl.get_unresolved_name());
    this_param->set_type(std::move(this_class_type));

    return this_param;
}
