#pragma once

#include <filesystem>
#include <vector>

namespace codesh
{

struct command_args
{
    std::filesystem::path src_path;
    std::filesystem::path dest_path;

    std::vector<std::filesystem::path> classpath;

    bool is_java_default_classpath = true; // disabled by --sinful
    bool is_talmud_codesh_classpath = true; // disabled by --unholy
};

command_args parse_command(int argc, char **argv);

}
