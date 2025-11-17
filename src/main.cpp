#include "command_parser.h"
#include "lexer/tokenizer.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "parser/ast/type_declaration/class_declaration_ast_node.h"
#include "parser/parser.h"
#include "test.h"

#include <filesystem>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>

static void add_default_constructors(const codesh::ast::compilation_unit_ast_node &root_node);
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

        class_decl->get_constructors().push_back(std::make_unique<codesh::ast::constructor_declaration_ast_node>());
    }
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