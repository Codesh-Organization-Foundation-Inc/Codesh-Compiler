#include "blasphemies/blasphemy_collector.h"
#include "command_parser.h"
#include "lexer/tokenizer.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "parser/parser.h"
#include "semantic_analyzer/analyzer.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

#include <filesystem>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>

static std::string read_file(const std::string &file_name);


int main(const int argc, char **const argv)
{
    const codesh::command_args args = codesh::parse_command(argc, argv);

    const std::string amen_file = read_file(std::string(args.src_path));


    //TESTS
    //codesh::test::descriptor();


    // LEXING
    auto tokens = codesh::lexer::tokenize_code(amen_file);

    // PARSING
    const auto ast = codesh::parser::parse(tokens, args.src_path.stem());

    // SEMANTIC ANALYZING
    codesh::semantic_analyzer::prepare(*ast);
    ast->construct_symbol_table();
    codesh::semantic_analyzer::analyze(*ast);

    if (codesh::error::get_blasphemy_collector().has_errors())
    {
        codesh::error::get_blasphemy_collector().print_all_blasphemies();
        return EXIT_FAILURE;
    }


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

    return EXIT_SUCCESS;
}

static std::string read_file(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        codesh::error::blasphemy_collector().add_blasphemy("לֹא נִתַּן לִפְתֹּחַ אֶת הַקּוֹבֶץ:  " + file_name,
            codesh::error::blasphemy_type::UNKNOWN, std::nullopt, true);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
