#include "command_parser.h"
#include "lexer/tokenizer.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "parser/parser.h"

#include <filesystem>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>

static void add_default_constructors(const codesh::ast::compilation_unit_ast_node &root_node);
static void add_this_param_to_non_static_methods(const codesh::ast::compilation_unit_ast_node &root_node);
static std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> create_this_param(
        const codesh::ast::type_decl::class_declaration_ast_node &class_decl);
static std::string read_file(const std::string &file_name);


int main(const int argc, char **const argv) {
    const codesh::command_args args = codesh::parse_command(argc, argv);

    const std::string amen_file = read_file(std::string(args.src_path));


    //TESTS
    //codesh::test::descriptor();
    //


    // LEXING
    auto tokens = codesh::lexer::tokenize_code(amen_file);

    // PARSING
    const auto ast = codesh::parser::parse(tokens, args.src_path.stem());

    //TODO: MOVE TO SEMANTIC ANALYZER
    add_default_constructors(*ast);
    add_this_param_to_non_static_methods(*ast);

    // A class file represents a single file.
    // So for each type declaration, build one class file:
    for (auto &type_declaration : ast->get_type_declarations())
    {
        // CONSTRUCTING CONSTANT POOLS
        type_declaration->set_constant_pool(codesh::output::jvm_target::constant_pool(*ast, *type_declaration));

        // BUILDING
        const auto class_file = codesh::output::jvm_target::class_file_builder(
            *ast,
            *type_declaration
        ).build();

        // WRITING
        codesh::output::jvm_target::write_to_file(*class_file, *ast, *type_declaration, args.dest_path);
    }

    return 0;
}

//TODO: MOVE TO SEMANTIC ANALYZER
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
        class_decl->get_constructors().push_back(std::move(constructor_decl));
    }
}
static void add_this_param_to_non_static_methods(const codesh::ast::compilation_unit_ast_node &root_node)
{
    for (const auto &type_decl : root_node.get_type_declarations())
    {
        const auto class_decl = dynamic_cast<codesh::ast::type_decl::class_declaration_ast_node *>(type_decl.get());
        if (!class_decl)
            continue;

        for (const auto &constructor_decl : class_decl->get_constructors())
        {
            // They must all be non-static anyway; Just add it.
            constructor_decl->get_parameters().push_front(
                create_this_param(*class_decl)
            );
        }

        for (const auto &method_decl : class_decl->get_methods())
        {
            if (!method_decl->get_attributes()->get_is_static())
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

std::unique_ptr<codesh::ast::method_declaration_ast_node> make_default_constructor()
{
    auto method_decl = std::make_unique<codesh::ast::method_declaration_ast_node>();

    // method_decl->set_name("<init>");
    // method_decl->

    return std::move(method_decl);
}

static std::string read_file(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        throw std::runtime_error("Couldn't open " + file_name);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}