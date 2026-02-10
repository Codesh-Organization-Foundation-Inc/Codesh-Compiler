#include "blasphemy_collector.h"

#include "fmt/xchar.h"

#include <iostream>
#include <random>
#include <utility>

static const std::vector<fmt::format_string<std::string>> RANDOM_MESSAGE_POOL = {
    "PLACEHOLDER_1 {}",
    "PLACEHOLDER_2 {}",
    "PLACEHOLDER_3 {}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "PLACEHOLDER_4 {}",
    "PLACEHOLDER_5 {}",
    "PLACEHOLDER_6 {}",
    "PLACEHOLDER_7 {}",
    "PLACEHOLDER_8 {}",
    "PLACEHOLDER_9 {}",
    "PLACEHOLDER_10 {}",
    "PLACEHOLDER_11 {}" //FIXME: Type is feminine while it shouldn't be here
};

static constexpr std::string PRETTY_PRINT_RED = "\033[31m";
static constexpr std::string PRETTY_PRINT_YELLOW = "\033[33m";
static constexpr std::string PRETTY_PRINT_END = "\033[0m";


void codesh::blasphemy::blasphemy_collector::add_blasphemy(std::string details, blasphemy_type type,
        code_position code_pos, const bool is_fatal)
{
    blasphemies.emplace_back(std::move(details), type, code_pos, is_fatal);

    if (is_fatal)
    {
        print_all_blasphemies();
        std::exit(EXIT_FAILURE);
    }
}

void codesh::blasphemy::blasphemy_collector::add_warning(std::string details, blasphemy_type type,
        code_position code_pos)
{
    warnings.emplace_back(std::move(details), type, code_pos, false);
}

void codesh::blasphemy::blasphemy_collector::set_source_directory(std::filesystem::path source_directory_path)
{
    this->source_directory_path = std::move(source_directory_path);
}

void codesh::blasphemy::blasphemy_collector::set_source_file(const std::filesystem::path &source_file_path)
{
    relative_source_path = std::filesystem::relative(source_file_path, source_directory_path);
}

bool codesh::blasphemy::blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void codesh::blasphemy::blasphemy_collector::print_blasphemy(const blasphemy_info &blasphemy,
        const std::string &color) const
{
    std::cerr << color;

    if (blasphemy.is_fatal)
    {
        std::cerr << "חֵטְא נוֹרָא: ";
    }

    std::cerr << get_blasphemy_message(blasphemy.type);

    fmt::print(stderr,
        " בְּסֵפֶר {}",
        relative_source_path.string()
    );

    if (const auto &code_pos = blasphemy.code_pos; code_pos->column != -1)
    {
        fmt::print(stderr,
            " פֶּרֶק {} פָּסוּק {}",
            std::to_string(code_pos->line),
            std::to_string(code_pos->column)
        );
    }

    fmt::println(stderr,
        ": {}",
        blasphemy.details
    );

    std::cerr << PRETTY_PRINT_END;
}

void codesh::blasphemy::blasphemy_collector::print_all_blasphemies() const
{
    for (const auto &blasphemy : blasphemies)
        print_blasphemy(blasphemy, PRETTY_PRINT_RED);

    for (const auto &warning : warnings)
        print_blasphemy(warning, PRETTY_PRINT_YELLOW);
}

std::string codesh::blasphemy::blasphemy_collector::type_to_string(const blasphemy_type type)
{
    switch (type)
    {
    case blasphemy_type::INIT: return "רִאשׁוֹנִית";
    case blasphemy_type::LEXICAL: return "לֶקְסִיקָלִית";
    case blasphemy_type::SEMANTIC: return "סֶמָנטִית";
    case blasphemy_type::SYNTAX: return "תַּחְבִּירִית";
    case blasphemy_type::OUTPUT: return "דְּבָרִים ל\"ד";
    case blasphemy_type::UNKNOWN: return "לֹא יְדוּעָה";

    default: throw std::runtime_error("Unknown blasphemy type");
    }
}

std::string codesh::blasphemy::blasphemy_collector::get_blasphemy_message(const blasphemy_type type)
{
    return fmt::format(get_random_message(), type_to_string(type));
}

fmt::format_string<std::string> codesh::blasphemy::blasphemy_collector::get_random_message()
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> dist(0, RANDOM_MESSAGE_POOL.size() - 1);

    return RANDOM_MESSAGE_POOL[dist(rng)];
}

codesh::blasphemy::blasphemy_collector &codesh::blasphemy::get_blasphemy_collector()
{
    static blasphemy_collector blasphemy_collector;
    return blasphemy_collector;
}
