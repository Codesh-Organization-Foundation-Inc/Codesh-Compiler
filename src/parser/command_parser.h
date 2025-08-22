#ifndef CODESH_COMPILER_COMMAND_PARSER_H
#define CODESH_COMPILER_COMMAND_PARSER_H

namespace codesh
{
    struct command_settings
    {
        char* src_path;
        char* dest_path;
    };

    command_settings parse_command(int argc, char** argv);

}


#endif // CODESH_COMPILER_COMMAND_PARSER_H
