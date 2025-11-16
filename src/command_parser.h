#pragma once

#include <filesystem>

namespace codesh
{

struct command_args
{
    std::filesystem::path src_path;
    std::filesystem::path dest_path;
};

command_args parse_command(int argc, char **argv);

}
