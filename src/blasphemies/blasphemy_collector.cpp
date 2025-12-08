#include "blasphemy_collector.h"

#include <iostream>
#include <random>
#include <utility>

static constexpr std::string TYPE_PLACEHOLDER = "{type}";

static const std::vector<std::string> RANDOM_MESSAGE_POOL = {
    "וְיִגְעַר ה' בַּיּוֹצֵר כִּי תּוֹעֵבָה {type} נוֹצְרָה",
    "וְיֶחֱרַד ה' כִּי רָאָה תּוֹעֵבָה {type} כְּאֵימַת קֹדֶשׁ",
    "וְיִכְעַס ה' כִּי הַיּוֹצֵר שָׂם מִלָּה וְשָׁכַח אֵחָרֵת כְּתוֹעֵבָה {type}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "וְיִצְעַק ה' אֶל הַיּוֹצֵר לֵאמֹר מֵי זֶה עֲשֵׂה תּוֹעֵבָה {type} זוֹ בִּמְקוֹמוֹ שֶׁל",
    "וְיִשְׁאַל ה' אֶל־הַיּוֹצֵר מַדּוּעַ בִּצַּעְתָּ תּוֹעֵבָה {type}",
    "וְיַזְהִיר ה' אֶת־הַיּוֹצֵר כִּי יְתַקֵּן בִּמְהֵרָה אֶת הַתּוֹעֵבָה הַ{type}",
    "וְיִתְאַכְזֵב ה' פֶּן רָאָה כִּי הַיּוֹצֵר אֵינוֹ תִּקֵּן אֶת הַתּוֹעֵבָה ה{type} הַשּׁוֹכֶנֶת",
    "וְקֶצֶף ה' בַּיּוֹצֵר תָמְלִיל הַקֹּדֶשׁ וְיֹאמַר אָחִי, תְּתַקֵּן אֶת הַתּוֹעֵבָה הַ{type} {type}",
    "תּוֹעֵבָה {type} נוֹצְרָה",
    "וְיֵאָנַח ה' וְיֹאמַר הֲלֹא אָמַרְתִּי וְהִנֵּה תּוֹעֵבָה {type}",
    "וְיִתְפַּלֵּא ה' אֶל הַיּוֹצֵר כִּי אֵיךְ הִנְּךָ לֹא הִבְחַנְתָּ בַּתּוֹעֵבָה הַ{type}",
    "וְיִפְתַּח ה' אֶת לוּחוֹת הַבְּרִית וְיִמְצָא הֲפָרָה בְּהַגֵּד ה{type}"
};

static constexpr std::string PRETTY_PRINT_BOLD = "\033[1;31m";
static constexpr std::string PRETTY_PRINT_END = "\033[0m";


void codesh::error::blasphemy_collector::add_blasphemy(std::string msg, blasphemy_type type,
        std::optional<code_position> code_pos, const bool is_fatal)
{
    blasphemies.emplace_back(std::move(msg), type, code_pos, is_fatal);

    if (is_fatal)
    {
        print_all_errors();
        std::exit(EXIT_FAILURE);
    }
}

bool codesh::error::blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void codesh::error::blasphemy_collector::print_all_errors() const
{
    for (const auto &blasphemy : blasphemies)
    {
        // TODO: move code_pos.value().line and file name
        std::cerr << get_blasphemy_message(blasphemy.type, 0, "דוגמה.אמן" );

        // TODO: Add column place


        std::cerr << blasphemy.message << std::endl;
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
            PRETTY_PRINT_BOLD + type_to_string(type) + PRETTY_PRINT_END
        );
    }

    // Blasphemies always provide the line position at the very end
    msg += "בְּפָּסוּק " + std::to_string(line) + PRETTY_PRINT_END;

    return msg + " עֲבוּר " + file_name + PRETTY_PRINT_END + ": ";
}

std::string codesh::error::blasphemy_collector::get_random_message()
{
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> dist(0, RANDOM_MESSAGE_POOL.size() - 1);

    return RANDOM_MESSAGE_POOL[dist(rng)];
}

codesh::error::blasphemy_collector &get_blasphemy_collector()
{
    static codesh::error::blasphemy_collector blasphemy_collector;
    return blasphemy_collector;
}
