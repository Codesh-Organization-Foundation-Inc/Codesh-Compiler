#include "jar_bundler.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "lexer/tokenizer.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdlib>
#include <filesystem>
#include <fmt/format.h>

static std::string finalize_command(std::string command);
static std::filesystem::path get_jar_cli_path(const std::filesystem::path &jre_path);
static bool move_jar_to_dest(const std::filesystem::path &temp_jar,
        const std::filesystem::path &dest_jar_path);


bool codesh::output::jvm_target::bundle_jar(const std::filesystem::path &temp_class_dir,
        const std::filesystem::path &dest_jar_path, const std::filesystem::path &jre_path)
{
    const auto jar_cli_path = get_jar_cli_path(jre_path);
    if (!std::filesystem::exists(jar_cli_path))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                blasphemy::details::PATH_DOESNT_EXIST,
                jar_cli_path.string()
            ),
            blasphemy::blasphemy_type::OUTPUT,
            lexer::NO_CODE_POS,
            true
        );
        return false;
    }

    // To avoid Hebrew path issues in the jar CLI, bundle in a temp path first.
    // Windows-only thing, but whatever.
    // build.ps1 does the same.
    const std::filesystem::path temp_jar =
        temp_class_dir.parent_path() / (temp_class_dir.filename().string() + ".jar");

    const auto command = finalize_command(fmt::format(
        R"("{}" cf "{}" -C "{}" .)",
        jar_cli_path.string(),
        temp_jar.string(),
        temp_class_dir.string()
    ));

    const int exit_code = std::system(command.c_str());
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

static bool move_jar_to_dest(const std::filesystem::path &temp_jar,
        const std::filesystem::path &dest_jar_path)
{
    std::error_code error;
    std::filesystem::rename(temp_jar, dest_jar_path, error);

    if (!error)
        return true;

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
            dest_jar_path.string()
        ),
        codesh::blasphemy::blasphemy_type::OUTPUT,
        codesh::lexer::NO_CODE_POS,
        true
    );
    return false;
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
