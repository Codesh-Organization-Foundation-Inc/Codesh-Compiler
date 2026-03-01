#include "command_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"

#include <queue>

static std::queue<std::string> create_args_queue(int argc, char **argv);
static std::string consume_argument(std::queue<std::string> &args);

static bool is_zip(const std::string &file_name);
static void parse_classpath(std::queue<std::string> &args, codesh::command_args &result);
static void parse_jre(std::queue<std::string> &args, codesh::command_args &result);
static std::filesystem::path get_default_jre_path();


codesh::command_args codesh::parse_command(const int argc, char **argv)
{
    command_args result {};
    result.is_jre = false;

    if (argc < 3)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_MAIN_ARGS,
            blasphemy::blasphemy_type::INIT,
            blasphemy::NO_CODE_POS,
            true
        );
    }

    auto args = create_args_queue(argc, argv);

    result.src_path = consume_argument(args);
    result.dest_path = consume_argument(args);

    while (!args.empty())
    {
        std::string arg = consume_argument(args);

        if (arg == "--classpath")
        {
            parse_classpath(args, result);
        }
        else if (arg == "--jre")
        {
            parse_jre(args, result);
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

    if (!result.is_jre)
    {
        result.jre_path = get_default_jre_path();
    }

    return result;
}

static void parse_classpath(std::queue<std::string> &args, codesh::command_args &result)
{
    if (args.empty())
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_ARG,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );
        return;
    }

    std::stringstream stream(consume_argument(args));
    std::string entry;

    while (std::getline(stream, entry, ';'))
    {
        if (entry.empty())
            continue;

        const std::filesystem::path file_path(entry);
        if (std::filesystem::is_directory(file_path) || is_zip(file_path.string()))
        {
            result.classpath.emplace_back(file_path);
            continue;
        }

        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(codesh::blasphemy::details::INVALID_ARG, entry),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS, false
        );
    }
}

static void parse_jre(std::queue<std::string> &args, codesh::command_args &result)
{
    if (args.empty())
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_ARG,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );
        return;
    }

    const std::filesystem::path folder_path(consume_argument(args));

    if (!std::filesystem::is_directory(folder_path))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(codesh::blasphemy::details::INVALID_ARG, folder_path.string()),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS, false
        );
    }

    result.is_jre = true;
    result.jre_path = folder_path.string();
}

static std::filesystem::path get_default_jre_path()
{
#ifdef _WIN32
    // Prefer JAVA_HOME if set
    if (const char* java_home = std::getenv("JAVA_HOME"))
    {
        return std::filesystem::path(java_home) / "jre";
    }
    // Common Windows jre path
    return "C:/Program Files/Java/jre-21";
#else
    // Common Linux / Unix jre path
    return "/usr/lib/jvm/jre-21";
#endif
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
    if (args.empty())
        return "";

    const auto arg_content = args.front();
    args.pop();

    return arg_content;
}

static std::queue<std::string> create_args_queue(const int argc, char **argv)
{
    std::queue<std::string> result;
    for (int i = 1; i < argc; ++i)
    {
        result.emplace(argv[i]);
    }
    return result;
}
