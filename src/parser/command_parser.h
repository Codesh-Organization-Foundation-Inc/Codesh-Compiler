#pragma once

#include <string_view>

namespace codesh
{

struct command_args
{
    std::string_view src_path;
    std::string_view dest_path;
};

command_args parse_command(int argc, char **argv);

}
