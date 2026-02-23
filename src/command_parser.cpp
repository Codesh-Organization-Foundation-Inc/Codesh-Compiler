#include "command_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"

#include <queue>

static std::queue<std::string> create_args_queue(int argc, char **argv);
static std::string consume_argument(std::queue<std::string> &args);

static bool is_zip(const std::string &file_name);


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

    //TODO: Implement
    auto args = create_args_queue(argc, argv);


    result.src_path = consume_argument(args);
    result.dest_path = consume_argument(args);

    if (argc == 3)
        return result;


    for (int i = 3; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--classpath")
        {
            if (i + 1 >= argc)
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(
                    blasphemy::details::NO_CLASSPATH_ARG,
                    blasphemy::blasphemy_type::INIT,
                    blasphemy::NO_CODE_POS,
                    true
                );
            }

            std::stringstream stream(argv[++i]);
            std::string entry;

            while (std::getline(stream, entry, ';'))
            {
                std::filesystem::path file_path(entry);

                if (std::filesystem::is_directory(file_path) || is_zip(file_path))
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
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::UNKNOWN_FLAG,
                blasphemy::blasphemy_type::INIT,
                blasphemy::NO_CODE_POS,
                true
            );
        }
    }

    return result;
}

static bool is_zip(const std::string &file_name)
{
    std::ifstream file;
    file.open(file_name, std::ios::binary);

    if (!file)
        return false;

    char magic[4];
    file.read(magic, 4);

    return magic[0] == 0x50 && magic[1] == 0x4B &&
           magic[2] == 0x03 && magic[3] == 0x04;
}

static std::string consume_argument(std::queue<std::string> &args)
{
    const auto arg_content = args.front();
    args.pop();

    return arg_content;
}

static std::queue<std::string> create_args_queue(const int argc, char **argv)
{
    std::queue<std::string> result;
    //TODO
    return result;
}
