#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "command_parser.h"
#include "defenition/definitions.h"
#include "lexer/tokenizer.h"
#include "lsp/lsp_receiver.h"
#include "output/jvm_target/class_file_builder.h"
#include "output/jvm_target/class_file_writer.h"
#include "output/jvm_target/jar_bundler.h"
#include "parser/parser.h"
#include "semantic_analyzer/analyzer.h"
#include "semantic_analyzer/builtins.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#endif
#include <chrono>
#include <filesystem>
#include <fmt/xchar.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct temp_dir_cleanup_guard
{
    const std::filesystem::path &dir;
    ~temp_dir_cleanup_guard();
};

static unsigned long get_pid();
[[nodiscard]] static std::filesystem::path make_temp_class_dir();

static int compile(const codesh::command_args &args, const codesh::definition::class_loaders &class_loaders);
[[noreturn]] static void lsp_server(const codesh::command_args &args,
        const codesh::definition::class_loaders &class_loaders);
static void handle_lsp_diagnostic_request(const codesh::command_args &args,
        const codesh::lsp::diagnostics_request &request, const codesh::definition::class_loaders &class_loaders);
[[nodiscard]] static std::filesystem::path uri_to_path(const std::string &file_uri);

static void print_tefilat_hahotsaa_besheela(const codesh::command_args &args);

[[nodiscard]] static std::vector<codesh::definition::fully_qualified_name> generate_default_imports(
        const codesh::command_args &args);
static codesh::definition::fully_qualified_name generate_default_import(const std::string &fqn_str);
static void update_source_file(size_t file_id);
static void update_source_file(const std::filesystem::path &source_file_path);
static void update_source_file(const codesh::ast::compilation_unit_ast_node &root_node);

[[nodiscard]] static codesh::definition::class_loaders init_class_loaders(
        const std::vector<std::filesystem::path> &classpaths);

[[nodiscard]] static std::string read_file(const std::filesystem::path &file_name);
[[nodiscard]] static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const codesh::command_args &args, const std::vector<std::filesystem::path> &source_files);
static void collect_source_files(const std::filesystem::path &path,
        std::vector<std::filesystem::path> &source_files_out);
static bool validate_output_path(const std::filesystem::path &dest_path, bool is_project);
[[nodiscard]] static bool build_and_bundle_jar(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::command_args &args, bool is_project, const codesh::semantic_analyzer::symbol_table &symbol_table);
[[nodiscard]] static std::optional<std::filesystem::path> get_output_path(const std::filesystem::path &cli_dest_path,
        const std::filesystem::path &sources_dir_path, const std::filesystem::path &source_file_path, bool is_project);

static void print_help();
static void log_analysis_progress(const codesh::command_args &args, size_t processed, size_t total,
        const std::string &pass_name, const codesh::ast::compilation_unit_ast_node &root_node);

static codesh::semantic_analyzer::symbol_table analyze_asts(const codesh::command_args &args,
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::definition::class_loaders &class_loaders);
static std::vector<codesh::semantic_analyzer::semantic_context> make_semantic_contexts(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        codesh::semantic_analyzer::symbol_table &table);
[[nodiscard]] static bool build_class_files(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::command_args &args, const std::filesystem::path &dest_path, bool is_project,
        const codesh::semantic_analyzer::symbol_table &symbol_table);
static void build_class_file(const codesh::ast::compilation_unit_ast_node &root_node,
        codesh::ast::type_decl::type_declaration_ast_node &type_decl, const std::filesystem::path &dest_path,
        const codesh::semantic_analyzer::symbol_table &symbol_table);

template <typename... T>
static void printfln(const codesh::command_args &args, fmt::format_string<T...> fmt, T&&... format_args);
static void println(const codesh::command_args &args, const std::string &msg);
static std::vector<std::string> fuck_windows();
[[nodiscard]] static codesh::command_args parse_args(int argc, char **argv,
        const std::vector<std::string> &utf8_args, std::vector<char *> &utf8_argv);


int main(const int argc, char **const argv)
{
    const auto utf8_args = fuck_windows();

    std::vector<char *> utf8_argv;
    const codesh::command_args args = parse_args(argc, argv, utf8_args, utf8_argv);

    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
    {
        codesh::blasphemy::get_blasphemy_collector().print_all_blasphemies();
        return EXIT_FAILURE;
    }

    // Initialize classloaders early on to get their blasphemies, if those exist
    const auto class_loaders = init_class_loaders(args.classpaths);

    if (args.help_requested)
    {
        print_help();
        return EXIT_SUCCESS;
    }

    if (args.lsp_mode)
    {
        lsp_server(args, class_loaders);
    }

    return compile(args, class_loaders);
}

static int compile(const codesh::command_args &args, const codesh::definition::class_loaders &class_loaders)
{
    if (!args.src_path)
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::SRC_NOT_PROVIDED,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::lexer::NO_CODE_POS,
            true
        );
        return EXIT_FAILURE;
    }
    if (!args.dest_path)
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::DEST_NOT_PROVIDED,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::lexer::NO_CODE_POS,
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


    println(args, "\n---------------------\n");
    const auto asts = parse_source_files(args, source_files);
    println(args, "");
    const auto master_symbol_table = analyze_asts(args, asts, class_loaders);
    println(args, "");


    // BLASPHEMIES
    // This includes both errors and warnings, so print it anyway
    codesh::blasphemy::get_blasphemy_collector().print_all_blasphemies();

    // Do NOT proceed with compilation if there were compilation errors
    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
    {
        return EXIT_FAILURE;
    }


    println(args, "וַיְחַל עֵת הַהוֹצָאָה בְּשֶׁאֵלֶּה\n");

    if (args.jar_output)
    {
        if (!build_and_bundle_jar(asts, args, is_project, master_symbol_table))
            return EXIT_FAILURE;
    }
    else
    {
        if (!validate_output_path(*args.dest_path, is_project))
            return EXIT_FAILURE;
        if (!build_class_files(asts, args, *args.dest_path, is_project, master_symbol_table))
            return EXIT_FAILURE;
    }

    println(args, "\n---------------------\n");
    println(args, "וַיִּשְׁבֹּת֙ הַמּוֹצִיא בִּשְׁאֵלָה מִכׇּל־מְלַאכְתּ֖וֹ אֲשֶׁ֥ר עָשׂה וַיֵּשֶׁב חָמָס וְיִתֹּם:");
    return EXIT_SUCCESS;
}


static void lsp_server(const codesh::command_args &args, const codesh::definition::class_loaders &class_loaders)
{
    while (true)
    {
        try
        {
            const auto request = codesh::lsp::wait_for_request();
            if (request == nullptr)
                continue;

            if (const auto diagnostics_req = dynamic_cast<const codesh::lsp::diagnostics_request *>(request.get()))
            {
                handle_lsp_diagnostic_request(args, *diagnostics_req, class_loaders);
            }
        }
        catch (...)
        {
            // skill issue segment
        }
    }
}

static void handle_lsp_diagnostic_request(const codesh::command_args &args,
        const codesh::lsp::diagnostics_request &request, const codesh::definition::class_loaders &class_loaders)
{
    const auto source_path = uri_to_path(request.file_uri);
    auto [tokens, file_id] = codesh::lexer::tokenize_code(source_path, request.file_contents);

    std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> asts;
    asts.reserve(1);
    asts.emplace_back(codesh::parser::parse(tokens, file_id));

    //TODO: Could be optimized if we cache the master AST and always remove the source and re-add it or something
    analyze_asts(args, asts, class_loaders);


    // Now that all the errors were collected, send them:
    codesh::lsp::send_diagnostics_response(request);
    
    // Clear for the next round
    codesh::blasphemy::get_blasphemy_collector().clear();
    codesh::lexer::get_global_source_info_map().clear();
}

constexpr std::string_view FILE_URI_PREFIX = "file://";

static std::filesystem::path uri_to_path(const std::string &file_uri)
{
    if (file_uri.starts_with(FILE_URI_PREFIX))
        return file_uri.substr(FILE_URI_PREFIX.size());

    return file_uri;
}


static void print_help()
{
    std::puts("Welcome to The Motzie B'Shelea - Official compiler to the Codesh programming language.");
    std::puts("");
    std::puts("This help message is written in English for all to understand. Your code, fortunately, is not.");
    std::puts("");
    std::puts("Usage:");
    std::puts("\tcodeshc --src <path> --dest <path> [options]");
    std::puts("");
    std::puts("Required:");
    std::puts("\t--src <path>                         Source file or directory to compile");
    std::puts("\t--dest <path>                        Output file or directory for .class files");
    std::puts("\t                                     If the path ends with .jar, outputs a bundled JAR");
    std::puts("");
    std::puts("Options:");
    std::puts("\t--sinful                             Include the standard Java library");
    std::puts("\t--unholy                             Exclude the standard Codesh talmud (not recommended)");
    fmt::println("\t--jre-path <path>                    Path to the JRE (default: {})", codesh::DEFAULT_JRE_PATH);
    std::puts("\t--classpath <entries>                Semicolon-separated list of classpaths to use (dirs or JARs)");
    fmt::println("\t--talmud-codesh-path <path>          Path to the Talmud Codesh standard library (default: {})", codesh::DEFAULT_TALMUD_CODESH_PATH);
    std::puts("\t--lsp                                Run in LSP Server Mode (for IDEs)");
    std::puts("");
    std::puts("JAR Target:");
    std::puts("\t--imashkha-kol-kakh-shmena           Makes the resulting JAR (almost) as fat as your mother by embedding all classpath entries within it.");
    std::puts("\t                                     Makes the JAR device-portable but bigger.");
    std::puts("\t--main-class <fqn>                   Fully-qualified name of the main class. Mandatory if more than one Bereshit method exists in a project");
    std::puts("");
    std::puts("\t--help, -h                           Show this help message");
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

static std::vector<codesh::definition::fully_qualified_name> generate_default_imports(const codesh::command_args &args)
{
    std::vector<codesh::definition::fully_qualified_name> results;

    // Always add Talmud Codesh import
    results.push_back(generate_default_import("ישראל/קודש/בן/משה"));

    if (args.is_java_default_classpath)
    {
        results.push_back(generate_default_import("java/lang"));
    }

    return results;
}

static codesh::definition::fully_qualified_name generate_default_import(const std::string &fqn_str)
{
    auto result = codesh::definition::fully_qualified_name::parse(
        fqn_str,
        codesh::lexer::NO_CODE_POS
    );
    result.set_is_wildcard(true);

    return result;
}

static void update_source_file(const size_t file_id)
{
    codesh::blasphemy::get_blasphemy_collector().set_source_file(file_id);
}
static void update_source_file(const codesh::ast::compilation_unit_ast_node &root_node)
{
    update_source_file(root_node.get_file_id());
}
static void update_source_file(const std::filesystem::path &source_file_path)
{
    codesh::blasphemy::get_blasphemy_collector().set_source_file(source_file_path);
}

static std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> parse_source_files(
        const codesh::command_args &args, const std::vector<std::filesystem::path> &source_files)
{
    println(args, "וְיַחֵל עֵת הַפֵּרוּשׁ הַתַּחְבִּירִי\n");
    std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> results;

    const auto process_amount = source_files.size();
    size_t processed = 1;

    for (const auto &source_path : source_files)
    {
        printfln(
            args,
            "{} מִן־{}: מְפָרֵשׁ אֶת {}",
            processed,
            process_amount,
            source_path.string()
        );
        update_source_file(source_path);

        // LEXING
        const std::string code = read_file(source_path);
        auto [tokens, file_id] = codesh::lexer::tokenize_code(source_path, code);

        // PARSING
        auto ast = codesh::parser::parse(tokens, file_id);

        results.push_back(std::move(ast));
        processed++;
    }

    return results;
}

static codesh::semantic_analyzer::symbol_table analyze_asts(
        const codesh::command_args &args,
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::definition::class_loaders &class_loaders)
{
    println(args, "וְיַחֵל עֵת הַנִּתּוּחַ הַסֵּמַנְטִי\n");

    codesh::semantic_analyzer::symbol_table master_symbol_table(class_loaders, generate_default_imports(args));
    // Add all builtins to the Talmud Codesh country
    codesh::semantic_analyzer::builtins::collect_builtins(master_symbol_table.get_talmud_codesh_country());


    const auto contexts = make_semantic_contexts(asts, master_symbol_table);

    const auto process_amount = asts.size() * 3; // 3 passes
    size_t processed = 1;

    for (const auto &context : contexts)
    {
        log_analysis_progress(args, processed, process_amount, "אחד", context.root);

        update_source_file(context.root);
        codesh::semantic_analyzer::collect_symbols(context);

        processed++;
    }

    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
        return master_symbol_table;

    // Methods require types to be resolved BEFORE they can be collected, but must also be collected
    // before analysis begins.
    //
    // This pass happens between symbol collection and analysis.
    if (args.is_java_default_classpath)
    {
        codesh::semantic_analyzer::builtins::resolve_builtins(
            master_symbol_table.get_talmud_codesh_country(),
            master_symbol_table
        );
    }

    for (const auto &context : contexts)
    {
        log_analysis_progress(args, processed, process_amount, "שתיים", context.root);

        update_source_file(context.root);
        codesh::semantic_analyzer::post_collect(context);

        processed++;
    }

    if (codesh::blasphemy::get_blasphemy_collector().has_errors())
        return master_symbol_table;

    for (const auto &context : contexts)
    {
        log_analysis_progress(args, processed, process_amount, "שלוש", context.root);

        update_source_file(context.root);
        codesh::semantic_analyzer::analyze(context);

        processed++;
    }

    return master_symbol_table;
}

static std::vector<codesh::semantic_analyzer::semantic_context> make_semantic_contexts(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        codesh::semantic_analyzer::symbol_table &table)
{
    std::vector<codesh::semantic_analyzer::semantic_context> contexts;
    contexts.reserve(asts.size());

    for (const auto &root_node : asts)
    {
        update_source_file(*root_node);
        codesh::semantic_analyzer::prepare(*root_node);
        contexts.push_back(codesh::semantic_analyzer::make_context(*root_node, table));
    }

    return contexts;
}

static codesh::definition::class_loaders init_class_loaders(const std::vector<std::filesystem::path> &classpaths)
{
    codesh::definition::class_loaders results;
    results.reserve(classpaths.size());

    for (const auto &classpath : classpaths)
    {
        if (auto class_loader = codesh::external::class_loader::create(classpath))
        {
            results.emplace(classpath, std::move(class_loader));
        }
    }

    return results;
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

static bool build_and_bundle_jar(
        const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::command_args &args, const bool is_project,
        const codesh::semantic_analyzer::symbol_table &symbol_table)
{
    const auto temp_dir = make_temp_class_dir();
    const temp_dir_cleanup_guard cleanup{temp_dir};

    if (build_class_files(asts, args, temp_dir, is_project, symbol_table))
    {
        return codesh::output::jvm_target::bundle_jar(
            symbol_table,
            {
                args.jre_path,
                temp_dir,
                *args.dest_path,
                args.explicit_main_class,
                args.classpaths,
                args.fat_jar,
            }
        );
    }

    return false;
}

static bool build_class_files(const std::vector<std::unique_ptr<codesh::ast::compilation_unit_ast_node>> &asts,
        const codesh::command_args &args, const std::filesystem::path &dest_path, const bool is_project,
        const codesh::semantic_analyzer::symbol_table &symbol_table)
{
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
                dest_path,
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

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        fmt::format(
            codesh::blasphemy::details::DEST_PATH_NOT_DIRECTORY,
            dest_path.string()
        ),
        codesh::blasphemy::blasphemy_type::INIT,
        codesh::lexer::NO_CODE_POS,
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
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR,
                source_file_path.string()
            ),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::lexer::NO_CODE_POS,
            true
        );

        return std::nullopt;
    }

    result /= relative_path.parent_path();
    std::filesystem::create_directories(result);
    return result;
}

static std::string read_file(const std::filesystem::path &file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR,
                file_name.string()
            ),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::lexer::NO_CODE_POS,
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

static codesh::command_args parse_args(const int argc, char **const argv,
        const std::vector<std::string> &utf8_args, std::vector<char *> &utf8_argv)
{
    if (utf8_args.empty())
        return codesh::parse_command(argc, argv);

    utf8_argv.reserve(utf8_args.size());
    for (const auto &arg : utf8_args)
    {
        utf8_argv.push_back(const_cast<char *>(arg.c_str()));
    }

    return codesh::parse_command(static_cast<int>(utf8_argv.size()), utf8_argv.data());
}

static std::vector<std::string> fuck_windows()
{
#ifdef _WIN32
    // 100% all-pure vibe coded slop solution
    // makes Hebrew & color coding work (i think)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    DWORD dwMode = 0;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
    GetConsoleMode(hErr, &dwMode);
    SetConsoleMode(hErr, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // argv is in the ANSI code page — get the real wide-char command line and convert to UTF-8
    int wargc;
    LPWSTR *wargv = CommandLineToArgvW(GetCommandLineW(), &wargc);
    if (wargv == nullptr)
        return {};

    std::vector<std::string> result;
    result.reserve(wargc);
    for (int i = 0; i < wargc; ++i)
    {
        const int size = WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, nullptr, 0, nullptr, nullptr);
        std::string utf8(size - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wargv[i], -1, utf8.data(), size, nullptr, nullptr);
        result.push_back(std::move(utf8));
    }

    LocalFree(wargv);
    return result;
#else
    return {};
#endif
}

temp_dir_cleanup_guard::~temp_dir_cleanup_guard()
{
    std::filesystem::remove_all(dir);
}

static unsigned long get_pid()
{
    return static_cast<unsigned long>(
#ifdef _WIN32
        _getpid()
#else
        getpid()
#endif
    );
}

static std::filesystem::path make_temp_class_dir()
{
    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const std::filesystem::path temp_class_dir =
        std::filesystem::temp_directory_path() / fmt::format("codesh-{}-{}", get_pid(), timestamp);

    std::filesystem::create_directories(temp_class_dir);
    return temp_class_dir;
}
