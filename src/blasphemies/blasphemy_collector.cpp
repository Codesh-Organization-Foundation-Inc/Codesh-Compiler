#include "blasphemy_collector.h"

#include <iostream>
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
        std::cerr << get_blasphemy_message(type, 0 ); // TODO: move code_pos.value().line too

        // TODO: Add column place


        std::cerr << "" << message << std::endl;
    }
}
std::string blasphemy_collector::type_to_string(const blasphemy_type type)
{
    switch (type)
    {
    case blasphemy_type::LEXICAL:
        return "לקסיקלית";
    case blasphemy_type::SEMANTIC:
        return "סמנטית";
    case blasphemy_type::SYNTAX:
        return "תחבירית";
    case blasphemy_type::UNKNOWN:
        return "לא ידועה";
    }

    throw std::runtime_error("Unknown type");
}
std::string blasphemy_collector::get_blasphemy_message(blasphemy_type type, size_t line) const
{
    std::string msg = get_random_message();

    // Replace {line} with bolded line number
    const std::string placeholder = "{line}";
    size_t index = msg.find(placeholder);
    if (index != std::string::npos)
        msg.replace(index, placeholder.size(), "\033[1;31mפסוק " + std::to_string(line) + "\033[0m");

    // Replace {type} with bolded error type
    const std::string type_placeholder = "{type}";
    index = msg.find(type_placeholder);
    if (index != std::string::npos)
        msg.replace(index, type_placeholder.size(), "\033[1;31m" + type_to_string(type) + "\033[0m");

    return msg + ": ";
}

std::string blasphemy_collector::get_random_message() const
{
    static const std::vector<std::string> messages = {

    "ויגער ה' ב־יוצר כי תועבה {type} נוצרה ב־{line}",
    "ויחרד ה' כי ראה טעות {type} כאימת קודש ב־{line}",
    "ויכעס ה' כי היוצר שם מילה ושכח אחרת כתועבה {type} ב־{line}", // NOT FOR ALL ERRORS BUT FINE FOR NOW
    "ויצעק ה' אל המקמפל לאמר מי זה עשה שגיאה {type} זו במקומו של {line}",
    "וישאל ה אל־היוצר מדוע ביצעת תועבה {type} ב־{line} ומדוע נוצר",
    "ויזהיר ה' את־היוצר כי יתקן מיד את התועבה ה{type} ב־{line}",
    "ויתאכזב ה' פן ראה כי ה־יוצר אינו תיקן את השגיאה ה{type} השוכנת ב־{line}",
    "ויקצף ה' ב־יוצר תמליל הקודש ויאמר אחי, תתקן את התועבה ה{type} ב־{type}",
    "תועבה {type} נוצרה ב־{line}",
    "ויאנח ה' ויאמר הלא אמרתי והנה תועבה {type} ב־{line}",
    "ויתפלא ה' אל ה־יוצר כי איך הינך לא הבחנת בתועבה ה{type} ב־{line}"

    // "ויפתח ה' את לוחות הברית ולא ימצא שם דבריך הנמצאים בשורה {line}"

    };

    unsigned long index = rand() % messages.size();
    return messages[index];
}


blasphemy_collector &get_blasphemy_collector()
{
    static blasphemy_collector blasphemy_collector;
    return blasphemy_collector;
}

} // namespace codesh::error