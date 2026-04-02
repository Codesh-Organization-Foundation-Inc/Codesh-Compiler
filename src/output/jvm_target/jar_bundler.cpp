#include "jar_bundler.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "classpath/loader/jar_loader.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

#include <ranges>

#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdlib>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

static constexpr size_t MANIFEST_MAX_LINE_BYTES = 72;

static bool get_main_class(const codesh::semantic_analyzer::symbol_table &symbol_table,
        codesh::semantic_analyzer::type_symbol **main_class_out);
static bool get_main_class_by_name(const codesh::semantic_analyzer::symbol_table &symbol_table,
        codesh::semantic_analyzer::type_symbol **main_class_out,
        const codesh::definition::fully_qualified_name &class_name);
static std::string path_str(const std::filesystem::path &path);
static std::string finalize_command(std::string command);
static std::filesystem::path get_jar_cli_path(const std::filesystem::path &jre_path);
static bool move_jar_to_dest(const std::filesystem::path &temp_jar,
        const std::filesystem::path &dest_jar_path);
static std::filesystem::path write_manifest(const std::filesystem::path &temp_dir,
        const codesh::semantic_analyzer::type_symbol *main_class,
        const codesh::definition::class_loaders &class_loaders);
static std::string build_jar_command(const std::filesystem::path &temp_jar, const std::filesystem::path &temp_class_dir,
        const std::filesystem::path &jar_cli_path, const std::filesystem::path *manifest_path);
static std::string build_class_path_manifest_entry(const codesh::definition::class_loaders &class_loaders);
static bool add_classpaths_to_dir(const codesh::definition::class_loaders &class_loaders,
        const std::filesystem::path &temp_class_dir, const std::filesystem::path &jar_cli_path);


bool codesh::output::jvm_target::bundle_jar(const semantic_analyzer::symbol_table &symbol_table,
        const jar_builder_context &context)
{
    const auto [
        jre_path,
        temp_class_dir,
        dest_jar_path,
        explicit_main_class,
        class_loaders,
        fat_jar
    ] = context;

    semantic_analyzer::type_symbol *main_class = nullptr;
    if (explicit_main_class.has_value())
    {
        if (!get_main_class_by_name(symbol_table, &main_class, *explicit_main_class))
            return false;
    }
    else if (!get_main_class(symbol_table, &main_class))
        return false;


    const auto jar_cli_path = get_jar_cli_path(jre_path);
    if (!std::filesystem::exists(jar_cli_path))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::PATH_DOESNT_EXIST,
                path_str(jar_cli_path)
            ),
            blasphemy::blasphemy_type::OUTPUT,
            lexer::NO_CODE_POS,
            true
        );
        return false;
    }


    if (fat_jar && !add_classpaths_to_dir(class_loaders, temp_class_dir, jar_cli_path))
        return false;


    // To avoid Hebrew path issues in the jar CLI, bundle in a temp path first.
    // Windows-only thing, but whatever.
    // build.ps1 does the same.
    const std::filesystem::path temp_jar =
        temp_class_dir.parent_path() / (temp_class_dir.filename().string() + ".jar");

    std::optional<std::filesystem::path> manifest_path;
    // In fat JAR mode, classpaths are already embedded.
    if (!fat_jar && (main_class != nullptr || !class_loaders.empty()))
    {
        manifest_path = write_manifest(temp_class_dir.parent_path(), main_class, class_loaders);
    }

    const auto command = build_jar_command(
        temp_jar,
        temp_class_dir,
        jar_cli_path,
        manifest_path ? &manifest_path.value() : nullptr
    );
    const int exit_code = std::system(command.c_str());

    if (manifest_path)
    {
        std::filesystem::remove(*manifest_path);
    }

    if (exit_code != 0)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::JAR_COMMAND_FAILED,
                exit_code
            ),
            blasphemy::blasphemy_type::OUTPUT,
            lexer::NO_CODE_POS,
            true
        );
        std::filesystem::remove(temp_jar);
        return false;
    }

    if (!move_jar_to_dest(temp_jar, dest_jar_path))
        return false;

    return true;
}

static bool get_main_class(const codesh::semantic_analyzer::symbol_table &symbol_table,
        codesh::semantic_analyzer::type_symbol **main_class_out)
{
    const auto &main_classes = symbol_table.get_main_classes();
    if (main_classes.empty())
        return true;

    // This function may only be called if the main-class parameter was not passed.
    // Therefore, there must only be ONE main class available to pull from.
    if (main_classes.size() == 1)
    {
        *main_class_out = &main_classes.front().get();
        return true;
    }

    // Otherwise, mark all as duplicated
    for (const auto &main_class : main_classes)
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::DUPLICATE_MAIN_CLASS,
            codesh::blasphemy::blasphemy_type::SEMANTIC,
            main_class.get().get_full_name().get_source_range(),
            true
        );
    }
    return false;
}

static bool get_main_class_by_name(const codesh::semantic_analyzer::symbol_table &symbol_table,
        codesh::semantic_analyzer::type_symbol **main_class_out,
        const codesh::definition::fully_qualified_name &class_name)
{
    for (const auto &main_class : symbol_table.get_main_classes())
    {
        if (main_class.get().get_full_name() == class_name)
        {
            *main_class_out = &main_class.get();
            return true;
        }
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        fmt::format(
            codesh::blasphemy::details::MAIN_CLASS_NOT_FOUND,
            class_name.holy_join()
        ),
        codesh::blasphemy::blasphemy_type::SEMANTIC,
        codesh::lexer::NO_CODE_POS,
        true
    );
    return false;
}

static void write_manifest_attribute(std::ofstream &manifest, std::string line)
{
    // Fold at 72 bytes per the JAR manifest spec
    while (line.size() > MANIFEST_MAX_LINE_BYTES)
    {
        manifest << line.substr(0, MANIFEST_MAX_LINE_BYTES) << "\r\n";
        // Continuation lines start with a single space
        line = ' ' + line.substr(MANIFEST_MAX_LINE_BYTES);
    }

    manifest << line << "\r\n";
}

static std::filesystem::path write_manifest(const std::filesystem::path &temp_dir,
        const codesh::semantic_analyzer::type_symbol *main_class,
        const codesh::definition::class_loaders &class_loaders)
{
    const auto manifest_path = temp_dir / "MANIFEST.MF";
    std::ofstream manifest(manifest_path, std::ios::binary);

    manifest << "Manifest-Version: 1.0\r\n";

    if (main_class != nullptr)
    {
        write_manifest_attribute(
            manifest,
            fmt::format(
                "Main-Class: {}",
                main_class->get_full_name().join()
            )
        );
    }

    if (!class_loaders.empty())
    {
        write_manifest_attribute(manifest, build_class_path_manifest_entry(class_loaders));
    }

    manifest << "\r\n";
    return manifest_path;
}

static std::string build_class_path_manifest_entry(const codesh::definition::class_loaders &class_loaders)
{
    std::vector<std::string> paths;
    paths.reserve(class_loaders.size());

    for (const auto &cp : class_loaders | std::ranges::views::keys)
    {
        paths.push_back(cp.generic_string());
    }

    return fmt::format("Class-Path: {}", fmt::join(paths, " "));
}

static std::string build_jar_command(const std::filesystem::path &temp_jar, const std::filesystem::path &temp_class_dir,
        const std::filesystem::path &jar_cli_path, const std::filesystem::path *manifest_path)
{
    std::string command;

    if (manifest_path != nullptr)
    {
        command = fmt::format(
            R"("{}" cfm "{}" "{}" -C "{}" .)",
            jar_cli_path.string(),
            temp_jar.string(),
            manifest_path->string(),
            temp_class_dir.string()
        );
    }
    else
    {
        command = fmt::format(
            R"("{}" cf "{}" -C "{}" .)",
            jar_cli_path.string(),
            temp_jar.string(),
            temp_class_dir.string()
        );
    }

    return finalize_command(command);
}

static bool move_jar_to_dest(const std::filesystem::path &temp_jar,
        const std::filesystem::path &dest_jar_path)
{
    std::error_code error;
    std::filesystem::remove(dest_jar_path, error);
    std::filesystem::rename(temp_jar, dest_jar_path, error);

    if (!error)
        return true;

    std::filesystem::create_directories(dest_jar_path.parent_path(), error);
    // Try copy + remove instead of rename
    std::filesystem::copy_file(
        temp_jar,
        dest_jar_path,
        std::filesystem::copy_options::overwrite_existing,
        error
    );

    if (!error)
    {
        std::filesystem::remove(temp_jar);
        return true;
    }

    codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
        fmt::format(
            codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR,
            path_str(dest_jar_path)
        ),
        codesh::blasphemy::blasphemy_type::OUTPUT,
        codesh::lexer::NO_CODE_POS,
        true
    );
    return false;
}

static std::string path_str(const std::filesystem::path &path)
{
#ifdef _WIN32
    const auto u8 = path.u8string();
    return {reinterpret_cast<const char*>(u8.data()), u8.size()};
#else
    return path.string();
#endif
}

static std::string finalize_command(std::string command)
{
    // On Windows, cmd.exe requires the whole compound command wrapped in an extra pair of quotes.
#ifdef _WIN32
    return fmt::format("\"{}\"", std::move(command));
#else
    return std::move(command);
#endif
}

static std::filesystem::path get_jar_cli_path(const std::filesystem::path &jre_path)
{
#ifdef _WIN32
    return jre_path / "bin" / "jar.exe";
#else
    return jre_path / "bin" / "jar";
#endif
}

static bool add_classpaths_to_dir(const codesh::definition::class_loaders &class_loaders,
        const std::filesystem::path &temp_class_dir, const std::filesystem::path &jar_cli_path)
{
    for (const auto &[cp, class_loader] : class_loaders)
    {
        if (cp == std::filesystem::path("."))
            continue;
        // We don't care to specify modules.
        // Assuming it's only native Java ones.
        if (dynamic_cast<const codesh::external::jimage_loader *>(class_loader.get()))
            continue;

        if (std::filesystem::is_directory(cp))
        {
            std::error_code exit_code;
            std::filesystem::copy(
                cp,
                temp_class_dir,
                std::filesystem::copy_options::recursive
                    | std::filesystem::copy_options::overwrite_existing,
                exit_code
            );

            if (exit_code)
            {
                codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                    fmt::format(codesh::blasphemy::details::OUTPUT_FILE_OPEN_ERROR, path_str(cp)),
                    codesh::blasphemy::blasphemy_type::OUTPUT,
                    codesh::lexer::NO_CODE_POS,
                    true
                );
                return false;
            }
        }
        else if (codesh::external::is_jar(cp))
        {
            // `jar xf` extracts to the current working directory.
            // So temporarily change to temp_class_dir:
            const auto curr_path = std::filesystem::current_path();
            const auto abs_cp = std::filesystem::absolute(cp);
            std::filesystem::current_path(temp_class_dir);

            const std::string cmd = finalize_command(
                fmt::format(R"("{}" xf "{}")", jar_cli_path.string(), abs_cp.string())
            );
            const int exit_code = std::system(cmd.c_str());

            std::filesystem::current_path(curr_path);

            if (exit_code != 0)
            {
                codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                    fmt::format(codesh::blasphemy::details::JAR_COMMAND_FAILED, exit_code),
                    codesh::blasphemy::blasphemy_type::OUTPUT,
                    codesh::lexer::NO_CODE_POS,
                    true
                );
                return false;
            }
        }
    }
    return true;
}
