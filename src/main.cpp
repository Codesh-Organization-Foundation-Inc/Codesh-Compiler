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
static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const std::vector<std::filesystem::path> &source_files);
static void collect_source_files(const std::filesystem::path &path, std::vector<std::filesystem::path> &source_files_out);

static void build_class_file(const codesh::ast::compilation_unit_ast_node &root_node,
        codesh::ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &dest_path);


int main(const int argc, char **const argv)
{
    const codesh::command_args args = codesh::parse_command(argc, argv);

    // Collect all source files in the project
    std::vector<std::filesystem::path> source_files;
    bool is_project;

    std::error_code error;
    if (std::filesystem::is_directory(args.src_path, error))
    {
        collect_source_files(args.src_path, source_files);
        is_project = true;
    }
    else
    {
        // We don't care about its file extension so long as the user forced this source file, I guess.
        source_files.push_back(args.src_path);
        is_project = false;
    }

    //TESTS
    //codesh::test::descriptor();


    const auto asts = parse_source_files(source_files);

    // SEMANTIC ANALYZING
    for (const auto &root_node : asts)
    {
        codesh::semantic_analyzer::prepare(*root_node);
        root_node->construct_symbol_table();
        codesh::semantic_analyzer::analyze(*root_node);
    }


    // Do not proceed with compilation if there were compilation errors
    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
    {
        codesh::blasphemy::get_blasphemy_collector().print_all_blasphemies();
        return EXIT_FAILURE;
    }


    // A class file represents a single file.
    // So for each type declaration, build one class file:
    for (const auto &root_node : asts)
    {
        for (auto &type_declaration : root_node->get_type_declarations())
        {
            build_class_file(*root_node, *type_declaration, args.dest_path); //FIXME: Dest path is for one file
            // Use is_project
        }
    }

    return EXIT_SUCCESS;
}

static void build_class_file(const codesh::ast::compilation_unit_ast_node &root_node,
        codesh::ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &dest_path)
{
    // CONSTRUCTING CONSTANT POOLS
    type_decl.set_constant_pool(codesh::output::jvm_target::constant_pool(root_node, type_decl));

    // BUILDING
    codesh::output::jvm_target::defs::class_file class_file;
    codesh::output::jvm_target::class_file_builder(
        class_file,
        root_node,
        type_decl
    ).build();

    // WRITING
    codesh::output::jvm_target::write_to_file(class_file, root_node, type_decl, dest_path);
}

static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const std::vector<std::filesystem::path> &source_files)
{
    std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> results;

    for (const auto &source_file_path : source_files)
    {
        // LEXING
        const std::string source_file = read_file(source_file_path);
        auto tokens = codesh::lexer::tokenize_code(source_file);

        // PARSING
        auto ast = codesh::parser::parse(tokens, source_file_path.stem());

        results.push_back(std::move(ast));
    }

    return results;
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
