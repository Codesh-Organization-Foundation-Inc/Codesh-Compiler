#include "blasphemy_collector.h"

#include <iostream>
#include <random>
#include <utility>

namespace codesh::error
{

void blasphemy_collector::add_blasphemy(std::string msg)
{
    add_blasphemy(std::move(msg), blasphemy_type::UNKNOWN);
}

void blasphemy_collector::add_blasphemy(std::string msg, blasphemy_type type)
{
    blasphemies.emplace_back(std::move(msg), type);
}

void blasphemy_collector::add_blasphemy(std::string msg, blasphemy_type type, code_position code_pos)
{
    blasphemies.emplace_back(std::move(msg), type, code_pos);
}

bool blasphemy_collector::has_errors() const
{
    return !blasphemies.empty();
}

void blasphemy_collector::print_all_errors() const
{
    for (const auto &[message,type, code_pos] : blasphemies)
    {
        // TODO: move code_pos.value().line and file name
        std::cerr << get_blasphemy_message(type, 0, "דוגמה.אמן" );

        // TODO: Add column place


        std::cerr << "" << message << std::endl;
    }
}
std::string blasphemy_collector::type_to_string(const blasphemy_type type)
{
    switch (type)
    {
    case blasphemy_type::LEXICAL:
        return "לֶקְסִיקָלִית";
    case blasphemy_type::SEMANTIC:
        return "סֶמָנטִית";
    case blasphemy_type::SYNTAX:
        return "תַּחְבִּירִית";
    case blasphemy_type::UNKNOWN:
        return "לֹא יְדוּעָה";
    }

    throw std::runtime_error("Unknown blasphemy type");
}

std::string blasphemy_collector::get_blasphemy_message(blasphemy_type type, size_t line,
                                                       const std::string &file_name)
{
    std::string msg = get_random_message();

    // Replace {line} with bolded line number
    const std::string line_placeholder = "{line}";
    size_t index = msg.find(line_placeholder);
    if (index != std::string::npos)
        msg.replace(index, line_placeholder.size(), "\033[1;31mפָּסוּק " + std::to_string(line) + "\033[0m");

    // Replace {type} with bolded error type
    const std::string type_placeholder = "{type}";
    index = msg.find(type_placeholder);
    if (index != std::string::npos)
        msg.replace(index, type_placeholder.size(), "\033[1;31m" + type_to_string(type) + "\033[0m");

    return msg +" עבור \033[1;31m" + file_name + "\033[0m: ";
}

std::string blasphemy_collector::get_random_message()
{
    static const std::vector<std::string> messages = {

    "וְיִגְעַר ה' בַּיּוֹצֵר כִּי תּוֹעֵבָה {type} נוֹצְרָה בְּ־{line}",
    "וְיֶחֱרַד ה' כִּי רָאָה תּוֹעֵבָה {type} כְּאֵימַת קֹדֶשׁ בְּ־{line}",
    "וְיִכְעַס ה' כִּי הַיּוֹצֵר שָׂם מִלָּה וְשָׁכַח אֵחָרֵת כְּתוֹעֵבָה {type} בְּ־{line}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "וְיִצְעַק ה' אֶל הַיּוֹצֵר לֵאמֹר מֵי זֶה עֲשֵׂה תּוֹעֵבָה {type} זוֹ בִּמְקוֹמוֹ שֶׁל {line}",
    "וְיִשְׁאַל ה' אֶל־הַיּוֹצֵר מַדּוּעַ בִּצַּעְתָּ תּוֹעֵבָה {type} בְּ־{line}",
    "וְיַזְהִיר ה' אֶת־הַיּוֹצֵר כִּי יְתַקֵּן בִּמְהֵרָה אֶת הַתּוֹעֵבָה הַ{type} בְּ־{line}",
    "וְיִתְאַכְזֵב ה' פֶּן רָאָה כִּי הַיּוֹצֵר אֵינוֹ תִּקֵּן אֶת הַתּוֹעֵבָה ה{type} הַשּׁוֹכֶנֶת בְּ־{line}",
    "וְקֶצֶף ה' בַּיּוֹצֵר תָמְלִיל הַקֹּדֶשׁ וְיֹאמַר אָחִי, תְּתַקֵּן אֶת הַתּוֹעֵבָה הַ{type} בְּ־{type}",
    "תּוֹעֵבָה {type} נוֹצְרָה בְּ־{line}",
    "וְיֵאָנַח ה' וְיֹאמַר הֲלֹא אָמַרְתִּי וְהִנֵּה תּוֹעֵבָה {type} בְּ־{line}",
    "וְיִתְפַּלֵּא ה' אֶל הַיּוֹצֵר כִּי אֵיךְ הִנְּךָ לֹא הִבְחַנְתָּ בַּתּוֹעֵבָה הַ{type} בְּ־{line}"

    // "ויפתח ה' את לוחות הברית ולא ימצא שם דבריך הנמצאים בשורה {line}"

    };

    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<size_t> dist(0, messages.size() - 1);

    return messages[dist(rng)];
}


blasphemy_collector &get_blasphemy_collector()
{
    static blasphemy_collector blasphemy_collector;
    return blasphemy_collector;
}

} // namespace codesh::error