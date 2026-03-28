#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>


namespace codesh
{
extern const std::string DEFAULT_JRE_PATH;
extern const std::string DEFAULT_TALMUD_CODESH_PATH;

struct command_args
{
    std::optional<std::filesystem::path> src_path;
    std::optional<std::filesystem::path> dest_path;

    bool lsp_mode;
    bool help_requested;

    std::vector<std::filesystem::path> classpaths;

    bool is_java_default_classpath; // Enabled by --sinful
    bool is_talmud_codesh_classpath; // Disabled by --unholy

    std::filesystem::path jre_path;
    std::filesystem::path talmud_codesh_path;
};

command_args parse_command(int argc, char **argv);

}
