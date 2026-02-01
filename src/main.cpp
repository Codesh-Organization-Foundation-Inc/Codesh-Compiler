#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "command_parser.h"
#include "lexer/tokenizer.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "parser/parser.h"
#include "semantic_analyzer/analyzer.h"
#include "semantic_analyzer/builtins.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "defenition/definitions.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static void update_source_file(const std::filesystem::path &source_file_path);
static void update_source_file(const codesh::ast::compilation_unit_ast_node &root_node);

static std::string read_file(const std::string &file_name);
static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const std::vector<std::filesystem::path> &source_files);
static void collect_source_files(const std::filesystem::path &path, std::vector<std::filesystem::path> &source_files_out);
static bool validate_output_path(const std::filesystem::path &dest_path, bool is_project);
static std::optional<std::filesystem::path> get_output_path(const std::filesystem::path &cli_dest_path,
        const std::filesystem::path &sources_dir_path, const std::filesystem::path &source_file_path, bool is_project);

static void build_class_file(const codesh::ast::compilation_unit_ast_node &root_node,
        codesh::ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &dest_path,
        const codesh::semantic_analyzer::symbol_table &symbol_table);

static codesh::semantic_analyzer::symbol_table build_master_symbol_table(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts);


int main(const int argc, char **const argv)
{
    const codesh::command_args args = codesh::parse_command(argc, argv);

    // Collect all source files in the project
    std::vector<std::filesystem::path> source_files;
    bool is_project;

    std::error_code error;
    if (std::filesystem::is_directory(args.src_path, error))
    {
        codesh::blasphemy::get_blasphemy_collector().set_source_directory(args.src_path);

        collect_source_files(args.src_path, source_files);
        is_project = true;
    }
    else
    {
        codesh::blasphemy::get_blasphemy_collector().set_source_directory(args.src_path.parent_path());

        // We don't care about its file extension so long as the user forced this source file, I guess.
        source_files.push_back(args.src_path);
        is_project = false;
    }

    //TESTS
    //codesh::test::descriptor();


    const auto asts = parse_source_files(source_files);

    // SEMANTIC ANALYZING
    const auto master_symbol_table = build_master_symbol_table(asts);

    for (const auto &root_node : asts)
    {
        update_source_file(*root_node);
        codesh::semantic_analyzer::prepare(*root_node);
        codesh::semantic_analyzer::collect_symbols(*root_node, master_symbol_table);
    }

    codesh::semantic_analyzer::builtins::add_builtins(master_symbol_table);

    // Collect all methods BEFORE analyzation and not during regular symbols collection as methods both need
    // types to be resolved in order to be collected but are also mandatory to be collected before analyzation
    // occurs.
    for (const auto &root_node : asts)
    {
        update_source_file(*root_node);
        codesh::semantic_analyzer::collect_methods(*root_node, master_symbol_table);
    }

    for (const auto &root_node : asts)
    {
        update_source_file(*root_node);
        codesh::semantic_analyzer::analyze(*root_node, master_symbol_table);
    }


    // Do not proceed with compilation if there were compilation errors
    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
    {
        codesh::blasphemy::get_blasphemy_collector().print_all_blasphemies();
        return EXIT_FAILURE;
    }


    if (!validate_output_path(args.dest_path, is_project))
        return EXIT_FAILURE;


    // A class file represents a single file.
    // So for each type declaration, build one class file:
    for (const auto &root_node : asts)
    {
        update_source_file(*root_node);
        const auto &source_file_path = root_node->get_source_path();

        for (auto &type_declaration : root_node->get_type_declarations())
        {
            const auto output_dir = get_output_path(
                args.dest_path,
                args.src_path,
                source_file_path,
                is_project
            );

            if (!output_dir.has_value())
            {
                return EXIT_FAILURE;
            }

            build_class_file(*root_node, *type_declaration, output_dir.value(), master_symbol_table);
        }
    }

    return EXIT_SUCCESS;
}

static void update_source_file(const std::filesystem::path &source_file_path)
{
    codesh::blasphemy::get_blasphemy_collector().set_source_file(source_file_path);
}
static void update_source_file(const codesh::ast::compilation_unit_ast_node &root_node)
{
    update_source_file(root_node.get_source_path());
}

static void build_class_file(const codesh::ast::compilation_unit_ast_node &root_node,
        codesh::ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &dest_path,
        const codesh::semantic_analyzer::symbol_table &symbol_table)
{
    // CONSTRUCTING CONSTANT POOLS
    type_decl.set_constant_pool(codesh::output::jvm_target::constant_pool(root_node, type_decl));

    // BUILDING
    codesh::output::jvm_target::defs::class_file class_file;
    codesh::output::jvm_target::class_file_builder(
        class_file,
        root_node,
        symbol_table,
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
        update_source_file(source_file_path);

        // LEXING
        const std::string source_file = read_file(source_file_path);
        auto tokens = codesh::lexer::tokenize_code(source_file);

        // PARSING
        auto ast = codesh::parser::parse(tokens, source_file_path);

        results.push_back(std::move(ast));
    }

    return results;
}

static codesh::semantic_analyzer::symbol_table build_master_symbol_table(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts)
{
    if (asts.empty())
        throw std::runtime_error("Cannot build a master symbol table without compilation units");

    return codesh::semantic_analyzer::symbol_table(*asts.front());
}

static bool validate_output_path(const std::filesystem::path &dest_path, const bool is_project)
{
    if (!is_project)
        return true;

    std::error_code error;
    if (std::filesystem::is_directory(dest_path, error))
        return true;

    //FIXME: Make this more specific to dest path not being a directory in a project-aligned compilation
    codesh::blasphemy::blasphemy_collector().add_blasphemy(
        codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR + dest_path.string(),
        codesh::blasphemy::blasphemy_type::INIT, codesh::blasphemy::NO_CODE_POS, true);

    return false;
}

static std::optional<std::filesystem::path> get_output_path(const std::filesystem::path &cli_dest_path,
        const std::filesystem::path &sources_dir_path, const std::filesystem::path &source_file_path,
        const bool is_project)
{
    std::filesystem::path result = cli_dest_path;

    // In a non-project, the CLI dest path IS the output path.
    if (!is_project)
        return result;


    // In a project, the CLI dest path is the output directory.
    std::error_code error;
    const auto relative_path = std::filesystem::relative(source_file_path, sources_dir_path, error);

    if (error)
    {
        codesh::blasphemy::blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR + source_file_path.string(),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );

        return std::nullopt;
    }

    result /= relative_path.parent_path();
    std::filesystem::create_directories(result);
    return result;
}

static std::string read_file(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        codesh::blasphemy::blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR + file_name,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );
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
