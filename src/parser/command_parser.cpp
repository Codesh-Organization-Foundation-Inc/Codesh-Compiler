struct command_settings
{
    char* src_path;
    char* dest_path;
};


command_settings parse_command(int argc, char** argv)
{
    command_settings result = { nllptr };

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