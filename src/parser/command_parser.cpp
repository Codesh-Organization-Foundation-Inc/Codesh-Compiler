#include "command_parser.h"
#include <stdexcept>

codesh::command_args codesh::parse_command(const int argc, char **argv)
{
    command_args result {};

    if (argc < 2)
        throw std::runtime_error("Missing arguments: Required input and output paths");

    result.src_path = argv[1];
    result.dest_path = argv[2];

    return result;
}
