#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "command_parser.h"
#include "defenition/definitions.h"
#include "lexer/tokenizer.h"
#include "lsp/lsp_receiver.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "parser/parser.h"
#include "semantic_analyzer/analyzer.h"
#include "semantic_analyzer/builtins.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

#include <filesystem>
#include <fmt/xchar.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <utf8.h>

static int compile(const codesh::command_args &args);
[[noreturn]] static void lsp_server(const codesh::command_args &args);

static void print_tefilat_hahotsaa_besheela(const codesh::command_args &args);

[[nodiscard]] static std::vector<std::string> generate_default_imports(const codesh::command_args &args);
static void update_source_file(const std::filesystem::path &source_file_path);
static void update_source_file(const codesh::ast::compilation_unit_ast_node &root_node);

[[nodiscard]] static std::string read_file(const std::string &file_name);
[[nodiscard]] static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const codesh::command_args &args, const std::vector<std::filesystem::path> &source_files);
static void collect_source_files(const std::filesystem::path &path,
        std::vector<std::filesystem::path> &source_files_out);
static bool validate_output_path(const std::filesystem::path &dest_path, bool is_project);
[[nodiscard]] static std::optional<std::filesystem::path> get_output_path(const std::filesystem::path &cli_dest_path,
        const std::filesystem::path &sources_dir_path, const std::filesystem::path &source_file_path, bool is_project);

static void log_analysis_progress(const codesh::command_args &args, size_t processed, size_t total,
        const std::string &pass_name, const codesh::ast::compilation_unit_ast_node &root_node);

[[nodiscard]] static codesh::semantic_analyzer::symbol_table analyze_asts(
        const codesh::command_args &args,
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts);
[[nodiscard]] static bool build_class_files(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::command_args &args, bool is_project,
        const codesh::semantic_analyzer::symbol_table &symbol_table);
static void build_class_file(const codesh::ast::compilation_unit_ast_node &root_node,
        codesh::ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &dest_path,
        const codesh::semantic_analyzer::symbol_table &symbol_table);

template <typename... T>
static void printfln(const codesh::command_args &args, fmt::format_string<T...> fmt, T&&... format_args);
static void println(const codesh::command_args &args, const std::string &msg);


int main(const int argc, char **const argv)
{
    const codesh::command_args args = codesh::parse_command(argc, argv);

    if (args.lsp_mode)
    {
        lsp_server(args);
    }

    return compile(args);
}

static int compile(const codesh::command_args &args)
{
    if (!args.src_path)
    {
        codesh::blasphemy::blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::SRC_NOT_PROVIDED,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );
        return EXIT_FAILURE;
    }
    if (!args.dest_path)
    {
        codesh::blasphemy::blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::DEST_NOT_PROVIDED,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );
        return EXIT_FAILURE;
    }

    println(args, "וַיָּחֶל הַמּוֹצִיא בְּשֶׁאֵלֶּה אֶת כׇּל־מְלַאכְתּוֹ");

    println(args, "");
    print_tefilat_hahotsaa_besheela(args);
    println(args, "\n");


    printfln(args, "מוֹצִיא מִן: {}", args.src_path->string());
    printfln(args, "אֶל: {}", args.dest_path->string());

    // Collect all source files in the project
    std::vector<std::filesystem::path> source_files;
    bool is_project;

    std::error_code error;
    if (std::filesystem::is_directory(*args.src_path, error))
    {
        println(args, "הַיַּעַד הִנּוֹ תִּיקִיָּה: וְיקַמְפֵּל אֶת כְּלָל־סִפְרֵי הַמָּקוֹר אֲשֶׁר בְּתוֹכוֹ");
        codesh::blasphemy::get_blasphemy_collector().set_source_directory(*args.src_path);

        collect_source_files(*args.src_path, source_files);
        is_project = true;
    }
    else
    {
        println(args, "הַיַּעַד הִנּוֹ סֵפֶר");
        codesh::blasphemy::get_blasphemy_collector().set_source_directory(args.src_path->parent_path());

        // We don't care about its file extension so long as the user forced this source file, I guess.
        source_files.push_back(*args.src_path);
        is_project = false;
    }


    //TESTS
    //codesh::test::descriptor();


    println(args, "\n---------------------\n");
    const auto asts = parse_source_files(args, source_files);
    println(args, "");
    const auto master_symbol_table = analyze_asts(args, asts);
    println(args, "");


    // BLASPHEMIES
    // This includes both errors and warnings, so print it anyway
    if (!args.lsp_mode)
    {
        codesh::blasphemy::get_blasphemy_collector().print_all_blasphemies();
    }
    else
    {
        //TODO: Print JSON
        return EXIT_SUCCESS;
    }

    // Do NOT proceed with compilation if there were compilation errors
    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
    {
        return EXIT_FAILURE;
    }


    if (!validate_output_path(*args.dest_path, is_project))
        return EXIT_FAILURE;
    if (!build_class_files(asts, args, is_project, master_symbol_table))
        return EXIT_FAILURE;


    println(args, "\n---------------------\n");
    println(args, "וַיִּשְׁבֹּת֙ הַמּוֹצִיא בִּשְׁאֵלָה מִכׇּל־מְלַאכְתּ֖וֹ אֲשֶׁ֥ר עָשׂה וַיֵּשֶׁב חָמָס וְיִתֹּם:");
    return EXIT_SUCCESS;
}

static void lsp_server(const codesh::command_args &)
{
    while (true)
    {
        codesh::lsp::wait_for_request();
    }
}

static void print_tefilat_hahotsaa_besheela(const codesh::command_args &args)
{
    println(args, "\tתְּפִלָּה לְעֵת הַהוֹצָאָה בְּשֶׁאֵלֶּה");
    println(args, "");
    println(args, "יְהִי רָצוֹן מִלְּפָנֶיךָ אֲדוֹן כָּל הַתַּחְבּוּלָה וּבוֹרֵא כָּל הַנְּתִיבוֹת:");
    println(args, "שֶׁיַּעֲלֶה מַעֲשֵׂה הַכְּתָב לְפָנֶיךָ כְּרֵיחַ נִיחוֹחַ וְלֹא יִמָּצֵא בּוֹ עָווֹן וְלֹא חֵטְא וְלֹא שְׁגִיאַת רֵיק מִתּוֹכֶן.");
    println(args, "הָאֵר עֵינַי בַּתָּנֶךְ שֶׁפָּתַחְתִּי לְמַעַן אֵדַע אֵימָתַי לִקְרֹא 'וַיֹּאמֶר' וְאֵימָתַי לַחְתֹּם 'וַיִּתֹּם' וְלֹא אֶחְטָא בְּגִלְגּוּל שֶׁאֵין לוֹ קֵץ.");
    println(args, "שְׁמֹר נָא עַל הַמַּחְסָנִית מִלְּהִתְמַלֵּא וְעַל הַזִּכָּרוֹן מִלִּדְלֹף. הַרְחֵק מִמֶּנִּי מְחַבְּלֵי קוֹד וּמַטְעֵי הַגְדָּרוֹת.");
    println(args, "וִיהִי נֹעַם הַמְּעַבֵּד עָלֵינוּ וּמַעֲשֵׂה יָדֵינוּ כּוֹנְנֵהוּ וּבְעֵת הַהַרָצָה יָאִיר לָנוּ הַמָּסָךְ בְּאוֹר הַהַצְלָחָה וְנֹאמַר אָמֵן.");
}

static std::vector<std::string> generate_default_imports(const codesh::command_args &args)
{
    std::vector<std::string> results;

    if (args.is_java_default_classpath)
    {
        results.emplace_back("java/lang");
    }
    if (args.is_talmud_codesh_classpath)
    {
        results.emplace_back("ישראל/קודש/בן/משה");
    }

    return results;
}

static void update_source_file(const std::filesystem::path &source_file_path)
{
    codesh::blasphemy::get_blasphemy_collector().set_source_file(source_file_path);
}
static void update_source_file(const codesh::ast::compilation_unit_ast_node &root_node)
{
    update_source_file(root_node.get_source_path());
}

static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const codesh::command_args &args, const std::vector<std::filesystem::path> &source_files)
{
    println(args, "וְיַחֵל עֵת הַפֵּרוּשׁ הַתַּחְבִּירִי\n");
    std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> results;

    const auto process_amount = source_files.size();
    size_t processed = 1;

    for (const auto &source_file_path : source_files)
    {
        printfln(
            args,
            "{} מִן־{}: מְפָרֵשׁ אֶת {}",
            processed,
            process_amount,
            source_file_path.string()
        );
        update_source_file(source_file_path);

        // LEXING
        const std::string source_file = read_file(source_file_path);
        // Convert the string to UTF-8.
        // Necessary because the compiler tokenizes non-ASCII characters (Hebrew and Maqaf)
        const std::u16string utf16_code = utf8::utf8to16(source_file);
        auto tokens = codesh::lexer::tokenize_code(utf16_code);

        // PARSING
        auto ast = codesh::parser::parse(tokens, source_file_path);

        results.push_back(std::move(ast));
        processed++;
    }

    return results;
}

static codesh::semantic_analyzer::symbol_table analyze_asts(
        const codesh::command_args &args,
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts)
{
    println(args, "וְיַחֵל עֵת הַנִּתּוּחַ הַסֵּמַנְטִי\n");

    codesh::semantic_analyzer::symbol_table master_symbol_table(args.classpaths, generate_default_imports(args));
    codesh::semantic_analyzer::builtins::collect_builtins(master_symbol_table);

    const auto process_amount = asts.size() * 3; // 3 passes
    size_t processed = 1;

    for (const auto &root_node : asts)
    {
        log_analysis_progress(args, processed, process_amount, "אחד", *root_node);

        update_source_file(*root_node);
        codesh::semantic_analyzer::prepare(*root_node);
        codesh::semantic_analyzer::collect_symbols(*root_node, master_symbol_table);

        processed++;
    }

    // Methods require types to be resolved BEFORE they can be collected, but must also be collected
    // before analysis begins.
    //
    // This pass happens between symbol collection and analysis.
    for (const auto &root_node : asts)
    {
        log_analysis_progress(args, processed, process_amount, "שתיים", *root_node);

        update_source_file(*root_node);
        codesh::semantic_analyzer::post_collect(*root_node, master_symbol_table);

        processed++;
    }

    for (const auto &root_node : asts)
    {
        log_analysis_progress(args, processed, process_amount, "שלוש", *root_node);

        update_source_file(*root_node);
        codesh::semantic_analyzer::analyze(*root_node, master_symbol_table);

        processed++;
    }

    return master_symbol_table;
}

static void log_analysis_progress(const codesh::command_args &args, const size_t processed, const size_t total,
        const std::string &pass_name, const codesh::ast::compilation_unit_ast_node &root_node)
{
    printfln(
        args,
        "נִתּוּחַ {} מִן־שְׁלֹשׁ: {} מן־{}: מְנַתֵּחַ אֶת {}",
        pass_name,
        processed,
        total,
        root_node.get_source_path().string()
    );
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
    codesh::output::jvm_target::write_to_file(class_file, type_decl, dest_path);
}

static bool build_class_files(const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::command_args &args, const bool is_project,
        const codesh::semantic_analyzer::symbol_table &symbol_table)
{
    println(args, "וַיְחַל עֵת הַהוֹצָאָה בְּשֶׁאֵלֶּה\n");

    const auto process_amount = asts.size();
    size_t processed = 1;

    // A class file represents a single file.
    // So for each type declaration, build one class file:
    for (const auto &root_node : asts)
    {
        printfln(
            args,
            "{} מִן־{}: מוֹצִיא בְּשֶׁאָלָה אֶת {}",
            processed,
            process_amount,
            root_node->get_source_path().string()
        );

        update_source_file(*root_node);
        const auto &source_file_path = root_node->get_source_path();

        for (auto &type_declaration : root_node->get_type_declarations())
        {
            const auto output_dir = get_output_path(
                *args.dest_path,
                *args.src_path,
                source_file_path,
                is_project
            );

            if (!output_dir.has_value())
                return false;

            build_class_file(*root_node, *type_declaration, output_dir.value(), symbol_table);
        }

        processed++;
    }

    return true;
}

static bool validate_output_path(const std::filesystem::path &dest_path, const bool is_project)
{
    if (!is_project)
        return true;

    std::error_code error;
    if (std::filesystem::is_directory(dest_path, error))
        return true;

    codesh::blasphemy::blasphemy_collector().add_blasphemy(
        fmt::format(
            codesh::blasphemy::details::DEST_PATH_NOT_DIRECTORY,
            dest_path.string()
        ),
        codesh::blasphemy::blasphemy_type::INIT,
        codesh::blasphemy::NO_CODE_POS,
        true
    );

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
            fmt::format(
                codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR,
                source_file_path.string()
            ),
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
            fmt::format(
                codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR,
                file_name
            ),
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

template <typename... T>
static void printfln(const codesh::command_args &args, fmt::format_string<T...> fmt, T&&... format_args)
{
    if (args.lsp_mode)
        return;

    fmt::println(fmt, std::forward<T>(format_args)...);
}

static void println(const codesh::command_args &args, const std::string &msg)
{
    if (args.lsp_mode)
        return;

    std::puts(msg.c_str());
}
