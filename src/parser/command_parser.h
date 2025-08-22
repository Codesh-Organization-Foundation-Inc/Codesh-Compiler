#pragma once

#include <string_view>

namespace codesh
{

struct command_settings
{
    std::string_view src_path;
    std::string_view dest_path;
};

command_settings parse_command(int argc, const char **argv);

}
