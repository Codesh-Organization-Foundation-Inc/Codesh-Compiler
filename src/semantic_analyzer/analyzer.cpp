#include "analyzer.h"

#include "../blasphemy/blasphemy_collector.h"
#include "../blasphemy/blasphemy_consumer.h"
#include "../parser/ast/method/operation/return_ast_node.h"
#include "../parser/ast/method/operation/super_call_ast_node.h"
#include "../parser/ast/type/primitive_type_ast_node.h"
#include "../parser/ast/type_declaration/attributes_ast_node.h"
#include "../parser/ast/type_declaration/class_declaration_ast_node.h"
#include "aliases.h"
#include "semantic_context.h"
#include "symbol_table/symbol.h"
#include "type_decl/resolve.h"
#include "type_decl/resolve_aliases.h"

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

const std::string codesh::semantic_analyzer::DEFAULT_SUPER_CLASS_NAME = "java/lang/Object";


void codesh::semantic_analyzer::prepare(const ast::compilation_unit_ast_node &ast_root)
{
    add_default_super_class(ast_root);

    add_default_constructor(ast_root);
    add_default_super_call(ast_root);

    add_default_return_statement(ast_root);

    add_this_param_to_non_static_methods(ast_root);
}

void codesh::semantic_analyzer::analyze(const ast::compilation_unit_ast_node &ast_root)
{
    //TODO: Use actual countries
    const std::vector lookup_countries = {
        ast_root.get_symbol_table().value().get().resolve_country("").value()
    };

    const semantic_context context = {lookup_countries, ast_root, blasphemy::semantic_consumer};

    const country_symbol &country = ast_root.get_symbol_table()->get().resolve_country("").value();


    type_declaration::resolve(context, country);

    //TODO: When CALLING non-static methods, also add 'this' as first argument

    type_declaration::resolve_aliases(context, country);
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
            const auto &body = constructor->get_body();

            // Only add super calls to those who lack it
            //TODO: Also filter "this" calls
            if (!body.empty() && dynamic_cast<const codesh::ast::method::operation::super_call_ast_node *>(body.front().get()))
                continue;

            constructor->get_body().emplace_back(
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


            const auto &body = method->get_body();

            //TODO: This SHOULD check all PATHS, not the last line alone.
            if (!body.empty() && dynamic_cast<const codesh::ast::method::operation::return_ast_node *>(body.front().get()))
                continue;

            method->get_body().emplace_back(
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

    auto attributes_node = std::make_unique<codesh::ast::type_decl::attributes_ast_node>();
    attributes_node->set_is_final(true);
    this_param->set_attributes(std::move(attributes_node));

    auto this_class_type = std::make_unique<codesh::ast::type::custom_type_ast_node>(class_decl.get_binary_name(false));
    this_param->set_type(std::move(this_class_type));

    return this_param;
}
