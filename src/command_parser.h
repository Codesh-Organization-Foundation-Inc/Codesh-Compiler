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

    std::filesystem::path jre_path;
};

command_args parse_command(int argc, char **argv);

}
