#include "blasphemy_collector.h"

#include "defenition/fully_qualified_name.h"
#include "fmt/xchar.h"

#include <iostream>
#include <random>
#include <utility>

static const std::vector<fmt::format_string<std::string>> RANDOM_MESSAGE_POOL = {
    "וְיִגְעַר ה' בַּיּוֹצֵר כִּי־תּוֹעֵבָה {} נוֹצְרָה",
    "וְיֶחֱרַד ה' כִּי־רָאָה תּוֹעֵבָה {} כְּאֵימַת קֹדֶשׁ",
    "וְיִכְעַס ה' כִּי הַיּוֹצֵר שָׂם מִלָּה וְשָׁכַח אֵחָרֵת כְּתוֹעֵבָה {}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "וַיַּרְא ה' הַיּוֹצֵר לֵאמֹר מֵי זֶה עֲשֵׂה תּוֹעֵבָה {}",
    "וְיִשְׁאַל ה' אֶת־יּוֹצֵר לֵאמֹר מַדּוּעַ בִּצַּעְתָּ תּוֹעֵבָה {}",
    "וְיַזְהִיר ה' אֶת יּוֹצֵר כִּי־יְתַקֵּן בִּמְהֵרָה אֶת הַתּוֹעֵבָה הַ{}",
    "וְיַחֲזֶה ה' בַּסֵפֶר לֵאמֹר וְיִתְאַכְזֵב וּמְאַכְזֵב יִהְיֶה פֶּן־רָאָה כִּי הַיּוֹצֵר לֹא תֶּקֶן אֶת הַתּוֹעֵבָה ה{} הַשּׁוֹכֶנֶת",
    "וְקֶצֶף ה' בְּיּוֹצֵר סֵפֶר הַקֹּדֶשׁ וְיֹאמַר לוֹ אָחִי, תַקֵּן אֶת הַתּוֹעֵבָה הַ{}",
    "וְיֵאָנַח ה' וְיֹאמַר הֲלֹא אָמַרְתִּי וְהִנֵּה תּוֹעֵבָה {}",
    "וְיִתְפַּלֵּא ה' בְּיּוֹצֵר כִּי־אֵיךְ לֹא הִבְחִין בַּתּוֹעֵבָה הַ{}",
    "וְיִפְתַּח ה' אֶת־לוּחוֹת הַבְּרִית וַיְהִי וְיִמְצָא הֲפָרָה בְּהַגֵּד הַ{}" //FIXME: Type is feminine while it shouldn't be here
};

static constexpr std::string PRETTY_PRINT_RED = "\033[31m";
static constexpr std::string PRETTY_PRINT_YELLOW = "\033[33m";
static constexpr std::string PRETTY_PRINT_END = "\033[0m";


void codesh::blasphemy::blasphemy_collector::add_blasphemy(std::string details, blasphemy_type type,
                                                           lexer::code_range source_range, bool is_fatal)
{
    blasphemies.emplace_back(std::move(details), type, file_id, relative_source_path, source_range, is_fatal);

    if (is_fatal)
    {
        print_all_blasphemies();
        std::exit(EXIT_FAILURE);
    }
}

void codesh::blasphemy::blasphemy_collector::add_blasphemy(std::string details, const blasphemy_type type,
                                                           const lexer::code_position code_start_pos,
                                                           const bool is_fatal)
{
    add_blasphemy(
        std::move(details),
        type,
        {
            code_start_pos,
            code_start_pos,
        },
        is_fatal
    );
}

void codesh::blasphemy::blasphemy_collector::add_warning(std::string details, blasphemy_type type,
                                                         lexer::code_range source_range)
{
    warnings.emplace_back(
        std::move(details),
        type,
        file_id,
        relative_source_path,
        source_range,
        false
    );
}

void codesh::blasphemy::blasphemy_collector::add_warning(std::string details, blasphemy_type type,
                                                         const lexer::code_position start_code_pos)
{
    add_warning(
        std::move(details),
        type,
        lexer::code_range {
            start_code_pos,
            start_code_pos
        }
    );
}

void codesh::blasphemy::blasphemy_collector::set_source_directory(std::filesystem::path source_directory_path)
{
    this->source_directory_path = std::move(source_directory_path);
}

void codesh::blasphemy::blasphemy_collector::set_source_file(const size_t file_id)
{
    const auto &[path, _] = lexer::get_global_source_info_map().at(file_id);

    this->file_id = file_id;
    this->relative_source_path = std::filesystem::relative(path, source_directory_path);
}

void codesh::blasphemy::blasphemy_collector::set_source_file(const std::filesystem::path &source_file_path)
{
    this->file_id = std::nullopt;
    this->relative_source_path = std::filesystem::relative(source_file_path, source_directory_path);
}

bool codesh::blasphemy::blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

const std::vector<codesh::blasphemy::blasphemy_info> &codesh::blasphemy::blasphemy_collector::get_all_blasphemies()
    const
{
    return blasphemies;
}

const std::vector<codesh::blasphemy::blasphemy_info> &codesh::blasphemy::blasphemy_collector::get_all_warnings() const
{
    return warnings;
}

void codesh::blasphemy::blasphemy_collector::clear()
{
    blasphemies.clear();
    warnings.clear();
}

void codesh::blasphemy::blasphemy_collector::print_blasphemy(const blasphemy_info &blasphemy, const std::string &color)
{
    std::cerr << color;

    if (blasphemy.is_fatal)
    {
        std::cerr << "חֵטְא נוֹרָא: ";
    }

    std::cerr << get_blasphemy_message(blasphemy.type);

    fmt::print(stderr,
        " בְּסֵפֶר {}",
        blasphemy.source_path.string()
    );

    if (const auto &code_pos = blasphemy.source_range)
    {
        const auto [line, column] = code_pos->start;

        fmt::print(stderr,
            " פֶּרֶק {} פָּסוּק {}",
            std::to_string(line),
            std::to_string(column)
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

bool codesh::lexer::code_position::operator==(const code_position &other) const
{
    return line == other.line && column == other.column;
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
