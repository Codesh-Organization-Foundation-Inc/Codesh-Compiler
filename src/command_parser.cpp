#include "command_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "output/jvm_target/class_file_builder.h"

#include <fstream>
#include <queue>

static std::queue<std::string> create_args_queue(int argc, char **argv);
static std::string consume_argument(std::queue<std::string> &args);
static std::string consume_string_argument(std::queue<std::string> &args);

static bool is_zip(const std::string &file_name);
static void parse_classpath(std::queue<std::string> &args, codesh::command_args &result);
static std::filesystem::path get_default_jre_path();
static void validate_jre_path(const codesh::command_args &args);

static void add_default_classpaths(codesh::command_args &args);

#ifdef _WIN32
const std::string LIB_PATH = "C:/Program Files/";
const std::string COMMON_JAVA_PATH = LIB_PATH + "Java/";
#else
const std::string LIB_PATH = "/usr/lib/";
const std::string COMMON_JAVA_PATH = LIB_PATH + "jvm/";
#endif


const std::string COMMON_JRE_DIR = "jre-" + std::to_string(codesh::output::jvm_target::JAVA_RELEASE_VERSION);
const std::string COMMON_TALMUD_CODESH_PATH = "קודש/תלמוד־קודש.jar";

const std::string codesh::DEFAULT_JRE_PATH = COMMON_JAVA_PATH + COMMON_JRE_DIR;
const std::string codesh::DEFAULT_TALMUD_CODESH_PATH = LIB_PATH + COMMON_TALMUD_CODESH_PATH;


codesh::command_args codesh::parse_command(const int argc, char **argv)
{
    command_args result{};

    if (argc == 1)
    {
        result.help_requested = true;
        return result;
    }

    result.is_java_default_classpath = false;
    result.is_talmud_codesh_classpath = true;

    auto args = create_args_queue(argc, argv);

    bool has_jre_path = false;
    bool has_talmud_codesh_path = false;

    while (!args.empty() & !result.help_requested)
    {
        std::string arg = consume_argument(args);

        if (arg == "--src")
        {
            result.src_path = consume_string_argument(args);
        }
        else if (arg == "--dest")
        {
            result.dest_path = consume_string_argument(args);
        }
        else if (arg == "--classpath")
        {
            parse_classpath(args, result);
        }
        else if (arg == "--jre-path")
        {
            has_jre_path = true;
            result.jre_path = consume_string_argument(args);
        }
        else if (arg == "--talmud-codesh-path")
        {
            has_talmud_codesh_path = true;
            result.talmud_codesh_path = consume_string_argument(args);
        }
        else if (arg == "--unholy")
        {
            result.is_talmud_codesh_classpath = false;
        }
        else if (arg == "--sinful")
        {
            result.is_java_default_classpath = true;
        }
        else if (arg == "--lsp")
        {
            result.lsp_mode = true;
        }
        else if (arg == "--help" || arg == "-h")
        {
            result.help_requested = true;
        }
        else
        {
            blasphemy::get_blasphemy_collector().add_blasphemy(
                blasphemy::details::UNKNOWN_FLAG,
                blasphemy::blasphemy_type::INIT,
                lexer::NO_CODE_POS,
                true
            );
        }
    }

    if (!has_jre_path)
    {
        result.jre_path = get_default_jre_path();
    }
    if (!has_talmud_codesh_path)
    {
        result.talmud_codesh_path = DEFAULT_TALMUD_CODESH_PATH;
    }

    result.jar_output = result.dest_path.has_value() && result.dest_path->extension() == ".jar";

    // Operations that require the JRE to exist:
    if (result.is_java_default_classpath || result.jar_output)
    {
        validate_jre_path(result);
    }

    add_default_classpaths(result);

    return result;
}

static void parse_classpath(std::queue<std::string> &args, codesh::command_args &result)
{
    std::stringstream stream(consume_string_argument(args));
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
            codesh::lexer::NO_CODE_POS
        );
    }
}

static std::filesystem::path get_default_jre_path()
{
#ifdef _WIN32
    // Prefer JAVA_HOME if set
    if (const char* java_home = std::getenv("JAVA_HOME"))
        return {java_home};
#endif
    return codesh::DEFAULT_JRE_PATH;
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
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_ARG,
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::lexer::NO_CODE_POS,
            true
        );
        throw std::runtime_error("");
    }

    const auto arg_content = args.front();
    args.pop();

    return arg_content;
}

static std::string consume_string_argument(std::queue<std::string> &args)
{
    std::string arg = consume_argument(args);
    if (arg.size() >= 2 && arg.front() == '"' && arg.back() == '"')
        return arg.substr(1, arg.size() - 2);

    return arg;
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

static void validate_jre_path(const codesh::command_args &args)
{
    if (!std::filesystem::exists(args.jre_path))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            fmt::format(
                codesh::blasphemy::details::PATH_DOESNT_EXIST,
                args.jre_path.string()
            ),
            codesh::blasphemy::blasphemy_type::INIT,
            codesh::lexer::NO_CODE_POS
        );
    }
}

static void add_default_classpaths(codesh::command_args &args)
{
    args.classpaths.emplace_back(".");

    if (args.is_java_default_classpath)
    {
        args.classpaths.emplace_back(args.jre_path / "lib/modules");
    }

    if (args.is_talmud_codesh_classpath)
    {
        if (!std::filesystem::exists(args.talmud_codesh_path))
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                fmt::format(
                    codesh::blasphemy::details::PATH_DOESNT_EXIST,
                    args.talmud_codesh_path.string()
                ),
                codesh::blasphemy::blasphemy_type::INIT,
                codesh::lexer::NO_CODE_POS
            );
        }
        else
        {
            args.classpaths.emplace_back(args.talmud_codesh_path);
        }
    }
}
