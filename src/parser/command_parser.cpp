#include <stdbool.h>
#include <stdio.h>

struct command_settings
{
    char* src_path;
    char* dest_path;
};


struct command_settings parse_command(int argc, char** argv)
{
    struct command_settings result = { 0 };

    if (argc < 2)
    {

    }
    else
    {
        result.src_path = argv[1];
        result.dest_path = argv[2];
    }

    return result;
}