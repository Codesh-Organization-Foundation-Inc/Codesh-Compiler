#include "blasphemy_collector.h"

#include <iostream>
#include <random>
#include <utility>

static constexpr std::string TYPE_PLACEHOLDER = "{type}";

static const std::vector<std::string> RANDOM_MESSAGE_POOL = {
    "וְיִגְעַר ה' בַּיּוֹצֵר כִּי תּוֹעֵבָה {type} נוֹצְרָה",
    "וְיֶחֱרַד ה' כִּי רָאָה תּוֹעֵבָה {type} כְּאֵימַת קֹדֶשׁ",
    "וְיִכְעַס ה' כִּי הַיּוֹצֵר שָׂם מִלָּה וְשָׁכַח אֵחָרֵת כְּתוֹעֵבָה {type}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "וַיַּרְא ה' הַיּוֹצֵר לֵאמֹר מֵי זֶה עֲשֵׂה תּוֹעֵבָה {type}",
    "וְיִשְׁאַל ה' אֶת־יּוֹצֵר לֵאמֹר מַדּוּעַ בִּצַּעְתָּ תּוֹעֵבָה {type}",
    "וְיַזְהִיר ה' אֶת יּוֹצֵר כִּי־יְתַקֵּן בִּמְהֵרָה אֶת־תּוֹעֵבָת הַ{type}",
    "וְיַחֲזֶה ה' בַּתַּמְלִיל לֵאמֹר וְיִתְאַכְזֵב וּמְאַכְזֵב יִהְיֶה פֶּן־רָאָה כִּי הַיּוֹצֵר לֹא תֶּקֶן אֶת הַתּוֹעֵבָה ה{type} הַשּׁוֹכֶנֶת",
    "וְקֶצֶף ה' בְּיּוֹצֵר תָמְלִיל הַקֹּדֶשׁ וְיֹאמַר לוֹ אָחִי, תַקֵּן אֶת הַתּוֹעֵבָה הַ{type}",
    "וְיֵאָנַח ה' וְיֹאמַר הֲלֹא אָמַרְתִּי וְהִנֵּה תּוֹעֵבָה {type}",
    "וְיִתְפַּלֵּא ה' בְּיּוֹצֵר כִּי־אֵיךְ לֹא הִבְחִין בַּתּוֹעֵבָה הַ{type}",
    "וְיִפְתַּח ה' אֶת־לוּחוֹת הַבְּרִית וַיְהִי וְיִמְצָא הֲפָרָה בְּהַגֵּד ה{type}" //FIXME: Type is feminine while it shouldn't be here
};

static constexpr std::string PRETTY_PRINT_RED = "\033[1;31m";
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

        if (blasphemy.is_fatal)
        {
            std::cerr << PRETTY_PRINT_RED << "חֵטְא נוֹרָא! " << PRETTY_PRINT_END;
        }

        // TODO: move code_pos.value().line and file name
        std::cerr << get_blasphemy_message(blasphemy.type, 0, "דוגמה.אמן");

        // TODO: Add column place

        std::cerr << blasphemy.details << std::endl;

    }
}
std::string codesh::error::blasphemy_collector::type_to_string(const blasphemy_type type)
{
    switch (type)
    {
    case blasphemy_type::LEXICAL: return "לֶקְסִיקָלִית";
    case blasphemy_type::SEMANTIC: return "סֶמָנטִית";
    case blasphemy_type::SYNTAX: return "תַּחְבִּירִית";
    case blasphemy_type::UNKNOWN: return "לֹא יְדוּעָה";

    default: throw std::runtime_error("Unknown blasphemy type");
    }
}

std::string codesh::error::blasphemy_collector::get_blasphemy_message(const blasphemy_type type, const size_t line,
                                                       const std::string &file_name)
{
    std::string msg = get_random_message();

    // Replace any {type} placeholders with the given type
    const size_t index = msg.find(TYPE_PLACEHOLDER);

    if (index != std::string::npos)
    {
        msg.replace(
            index, TYPE_PLACEHOLDER.size(),
            type_to_string(type)
        );
    }

    // Blasphemies always provide the line position at the very end
    msg += PRETTY_PRINT_RED + " בְּפָּסוּק " + std::to_string(line) + PRETTY_PRINT_END;

    return msg + " עֲבוּר " + PRETTY_PRINT_RED + file_name + ": " + PRETTY_PRINT_END;
}

std::string codesh::error::blasphemy_collector::get_random_message()
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
