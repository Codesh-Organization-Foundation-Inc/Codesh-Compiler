#include "command_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "output/jvm_target/class_file_builder.h"

#include <queue>

static std::queue<std::string> create_args_queue(int argc, char **argv);
static std::string consume_argument(std::queue<std::string> &args);

static bool is_zip(const std::string &file_name);
static void parse_classpath(std::queue<std::string> &args, codesh::command_args &result);
static std::filesystem::path parse_directory_path(std::queue<std::string> &args);
static std::filesystem::path get_default_jre_path();

static void add_default_classpaths(codesh::command_args &result);

const std::string LIB_PATH =
#ifdef _WIN32
    "C:/Program Files/Java/"
#else
    "/usr/lib/jvm/"
#endif
;

const std::string COMMON_JRE_DIR = "jre-" + std::to_string(codesh::output::jvm_target::JAVA_RELEASE_VERSION);
const std::string COMMON_TALMUD_CODESH_PATH = "קודש/תלמוד־קודש";

const std::string DEFAULT_JRE_PATH = LIB_PATH + COMMON_JRE_DIR;
const std::string DEFAULT_TALMUD_CODESH_PATH = LIB_PATH + COMMON_TALMUD_CODESH_PATH;


codesh::command_args codesh::parse_command(const int argc, char **argv)
{
    command_args result{};
    result.is_java_default_classpath = false;
    result.is_talmud_codesh_classpath = true;

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

    bool has_jre_path = false;
    bool has_talmud_codesh_path = false;

    while (!args.empty())
    {
        std::string arg = consume_argument(args);

        if (arg == "--classpath")
        {
            parse_classpath(args, result);
        }
        else if (arg == "--jre-path")
        {
            has_jre_path = true;
            result.jre_path = parse_directory_path(args).string();
        }
        else if (arg == "--talmud-codesh-path")
        {
            has_talmud_codesh_path = true;
            result.talmud_codesh_path = parse_directory_path(args).string();
        }
        else if (arg == "--unholy")
        {
            result.is_talmud_codesh_classpath = false;
        }
        else if (arg == "--sinful")
        {
            result.is_java_default_classpath = true;
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
    add_default_classpaths(result);

    if (!has_jre_path)
    {
        result.jre_path = get_default_jre_path();
    }
    if (!has_talmud_codesh_path)
    {
        result.talmud_codesh_path = get_default_jre_path();
    }
    //TODO: Check whether the paths truly exist

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
            result.classpaths.emplace_back(file_path);
            continue;
        }

        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(codesh::blasphemy::details::INVALID_ARG, entry),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS, false
        );
    }
}

static std::filesystem::path parse_directory_path(std::queue<std::string> &args)
{
    if (args.empty())
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_ARG,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::blasphemy::NO_CODE_POS,
            true
        );
        throw std::runtime_error("");
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

    return folder_path;
}

static std::filesystem::path get_default_jre_path()
{
#ifdef _WIN32
    // Prefer JAVA_HOME if set
    if (const char* java_home = std::getenv("JAVA_HOME"))
    {
        return std::filesystem::path(java_home) / COMMON_JRE_DIR;
    }
#endif
    return DEFAULT_JRE_PATH;
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

static void add_default_classpaths(codesh::command_args &result)
{
    result.classpaths.emplace_back(".");

    if (result.is_java_default_classpath)
    {
        result.classpaths.emplace_back(get_default_jre_path() / "lib/modules");
    }

    if (result.is_talmud_codesh_classpath)
    {
        //FIXME: Not the path
        // const std::filesystem::path talmud_path = "../resources/lib-src/ישראל";
        //
        // //TODO: Move to yet-to-exist check after this function is run
        // if (std::filesystem::exists(talmud_path))
        // {
        //     result.classpaths.emplace_back(talmud_path);
        // }
        // else
        // {
        //     throw std::runtime_error("talmud path was not found");
        // }
    }
}