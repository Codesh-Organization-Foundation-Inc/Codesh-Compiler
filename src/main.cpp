#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "command_parser.h"
#include "lexer/tokenizer.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "parser/parser.h"
#include "semantic_analyzer/analyzer.h"
#include "defenition/definitions.h"

#include <filesystem>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>

static std::string read_file(const std::string &file_name);
static void collect_source_files(const std::filesystem::path &path, std::vector<std::filesystem::path> &source_files_out);

int main(const int argc, char **const argv)
{
    const codesh::command_args args = codesh::parse_command(argc, argv);

    // Collect all source files in the project
    std::vector<std::filesystem::path> source_files;

    std::error_code error;
    if (std::filesystem::is_directory(args.src_path, error))
    {
        collect_source_files(args.src_path, source_files);
    }
    else
    {
        // We don't care about its file extension so long as the user forced this source file, I guess.
        source_files.push_back(args.src_path);
    }

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

    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
    {
        codesh::blasphemy::get_blasphemy_collector().print_all_blasphemies();
        return EXIT_FAILURE;
    }


    // A class file represents a single file.
    // So for each type declaration, build one class file:
    for (auto &type_declaration : ast->get_type_declarations())
    {
        // CONSTRUCTING CONSTANT POOLS
        type_declaration->set_constant_pool(codesh::output::jvm_target::constant_pool(*ast, *type_declaration));

        // BUILDING
        codesh::output::jvm_target::defs::class_file class_file;
        codesh::output::jvm_target::class_file_builder(
            class_file,
            *ast,
            *type_declaration
        ).build();

        // WRITING
        codesh::output::jvm_target::write_to_file(class_file, *ast, *type_declaration, args.dest_path);
    }

    return EXIT_SUCCESS;
}

static std::string read_file(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        codesh::blasphemy::blasphemy_collector().add_blasphemy(codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR
            + file_name,
            codesh::blasphemy::blasphemy_type::INIT, std::nullopt, true);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

static void collect_source_files(const std::filesystem::path &path, std::vector<std::filesystem::path> &source_files_out)
{
    if (std::filesystem::is_directory(path))
    {
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            collect_source_files(entry.path(), source_files_out);
        }

        return;
    }

    if (!std::filesystem::is_regular_file(path))
        return;

    if (path.extension() != codesh::definition::SOURCE_FILE_EXTENSION)
        return;


    source_files_out.push_back(path);
}
