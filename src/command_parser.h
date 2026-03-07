#pragma once

#include <filesystem>
#include <vector>

namespace codesh
{

struct command_args
{
    std::filesystem::path src_path;
    std::filesystem::path dest_path;

    std::vector<std::filesystem::path> classpaths;

    bool is_java_default_classpath; // Enabled by --sinful
    bool is_talmud_codesh_classpath; // Disabled by --unholy

    std::filesystem::path jre_path;
};

command_args parse_command(int argc, char **argv);

}
