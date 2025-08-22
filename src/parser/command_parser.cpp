#include "command_parser.h"
#include <stdexcept>

codesh::command_settings codesh::parse_command(const int argc, const char **argv)
{
    command_settings result {};

    if (argc < 2)
        throw std::runtime_error("Missing arguments: need input and output file");

    result.src_path = argv[1];
    result.dest_path = argv[2];

    return result;
}