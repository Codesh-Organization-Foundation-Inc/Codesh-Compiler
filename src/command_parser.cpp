#include "command_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"

codesh::command_args codesh::parse_command(const int argc, char **argv)
{
    command_args result {};

    if (argc < 3)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_MAIN_ARGS,
            blasphemy::blasphemy_type::INIT,
            blasphemy::NO_CODE_POS,
            true
        );
    }

    result.src_path = argv[1];
    result.dest_path = argv[2];

    if (argc == 3)
    {
        return result;
    }

    for (int i = 3; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--classpath")
        {
            if (i + 1 >= argc)
                blasphemy::get_blasphemy_collector().add_blasphemy(
                    blasphemy::details::NO_CLASSPATH_ARG,
                    blasphemy::blasphemy_type::INIT,
                    blasphemy::NO_CODE_POS,
                    true
                );

            std::stringstream stream(argv[++i]);
            std::string entry;

            while (std::getline(stream, entry, ':'))
            {
                std::filesystem::path file_path(entry);

                if (std::filesystem::is_directory(file_path)) // add is_zip check
                {
                    result.classpath.push_back(file_path);
                }
                else
                {
                    blasphemy::get_blasphemy_collector().add_blasphemy(
                        fmt::format(
                            blasphemy::details::INVALID_CLASSPATH_ARG,
                            entry
                        ),
                        blasphemy::blasphemy_type::INIT,
                        blasphemy::NO_CODE_POS,
                        false
                    );
                }
            }
        }
        else
        {
            // add error
        }
    }

    return result;
}