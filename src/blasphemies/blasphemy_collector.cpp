#include "blasphemy_collector.h"

#include "fmt/os.h"

#include <format>
#include <iostream>
#include <random>
#include <utility>

static const std::vector<fmt::format_string<std::string>> RANDOM_MESSAGE_POOL = {
    "וְיִגְעַר ה' בַּיּוֹצֵר כִּי תּוֹעֵבָה {} נוֹצְרָה",
    "וְיֶחֱרַד ה' כִּי רָאָה תּוֹעֵבָה {} כְּאֵימַת קֹדֶשׁ",
    "וְיִכְעַס ה' כִּי הַיּוֹצֵר שָׂם מִלָּה וְשָׁכַח אֵחָרֵת כְּתוֹעֵבָה {}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "וַיַּרְא ה' הַיּוֹצֵר לֵאמֹר מֵי זֶה עֲשֵׂה תּוֹעֵבָה {}",
    "וְיִשְׁאַל ה' אֶת־יּוֹצֵר לֵאמֹר מַדּוּעַ בִּצַּעְתָּ תּוֹעֵבָה {}",
    "וְיַזְהִיר ה' אֶת יּוֹצֵר כִּי־יְתַקֵּן בִּמְהֵרָה אֶת הַתּוֹעֵבָה הַ{}",
    "וְיַחֲזֶה ה' בַּתַּמְלִיל לֵאמֹר וְיִתְאַכְזֵב וּמְאַכְזֵב יִהְיֶה פֶּן־רָאָה כִּי הַיּוֹצֵר לֹא תֶּקֶן אֶת הַתּוֹעֵבָה ה{} הַשּׁוֹכֶנֶת",
    "וְקֶצֶף ה' בְּיּוֹצֵר תָמְלִיל הַקֹּדֶשׁ וְיֹאמַר לוֹ אָחִי, תַקֵּן אֶת הַתּוֹעֵבָה הַ{}",
    "וְיֵאָנַח ה' וְיֹאמַר הֲלֹא אָמַרְתִּי וְהִנֵּה תּוֹעֵבָה {}",
    "וְיִתְפַּלֵּא ה' בְּיּוֹצֵר כִּי־אֵיךְ לֹא הִבְחִין בַּתּוֹעֵבָה הַ{}",
    "וְיִפְתַּח ה' אֶת־לוּחוֹת הַבְּרִית וַיְהִי וְיִמְצָא הֲפָרָה בְּהַגֵּד ה{}" //FIXME: Type is feminine while it shouldn't be here
};

static constexpr std::string PRETTY_PRINT_RED = "\033[31m";
static constexpr std::string PRETTY_PRINT_END = "\033[0m";


void codesh::error::blasphemy_collector::add_blasphemy(std::string details, blasphemy_type type,
        std::optional<code_position> code_pos, const bool is_fatal)
{
    blasphemies.emplace_back(std::move(details), type, code_pos, is_fatal);

    if (is_fatal)
    {
        print_all_blasphemies();
        std::exit(EXIT_FAILURE);
    }
}

bool codesh::error::blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void codesh::error::blasphemy_collector::print_all_blasphemies() const
{
    for (const auto &blasphemy : blasphemies)
    {
        std::cerr << PRETTY_PRINT_RED;

        if (blasphemy.is_fatal)
        {
            std::cerr << "חֵטְא נוֹרָא: ";
        }

        std::cerr << get_blasphemy_message(blasphemy.type);

        if (blasphemy.code_pos.has_value())
        {
            fmt::println(stderr,
                " בְּפָּסוּק {}",
                std::to_string(blasphemy.code_pos->line)
            );
        }

        fmt::println(stderr,
            " עֲבוּר {}: {}",

            // TODO: Add file name to blasphemy
            "דוגמה.אמן",
            blasphemy.details
        );

        std::cerr << PRETTY_PRINT_END;
    }
}
std::string codesh::error::blasphemy_collector::type_to_string(const blasphemy_type type)
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

std::string codesh::error::blasphemy_collector::get_blasphemy_message(const blasphemy_type type)
{
    return fmt::format(get_random_message(), type_to_string(type));
}

fmt::format_string<std::string> codesh::error::blasphemy_collector::get_random_message()
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> dist(0, RANDOM_MESSAGE_POOL.size() - 1);

    return RANDOM_MESSAGE_POOL[dist(rng)];
}

codesh::error::blasphemy_collector &codesh::error::get_blasphemy_collector()
{
    static blasphemy_collector blasphemy_collector;
    return blasphemy_collector;
}
