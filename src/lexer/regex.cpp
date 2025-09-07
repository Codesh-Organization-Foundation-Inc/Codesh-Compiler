#include "regex.h"

#include "../token/token.h"

#include <format>
#include <sstream>
#include <vector>

using codesh::token_group;


// The order of this vector determines the order of regex priority & group number.
static const std::vector<std::pair<token_group, std::string>> TOKEN_REGEXES = {
    {token_group::COMMENT_MULTILINE, R"(כה הגה ה' לאמר:(?:.|\s)*ויחדל\:)"},
    {token_group::COMMENT_ONE_LINER, R"(כה הגה ה' לאמר [^\n]*)"},

    {token_group::PUNCTUATION_ARG_SEPARATOR, R"(\bו־)"},
    {token_group::PUNCTUATION_DOT, R"(\bל־)"},
    {token_group::PUNCTUATION_WILDCARD, R"(\bכולל\b)"},

    {token_group::LITERAL_NUMBER_FLOAT, R"(\d+(?:\.\d+)?\s+צף)"},
    {token_group::LITERAL_NUMBER_DOUBLE, R"(\d+\.\d+)"},
    {token_group::LITERAL_NUMBER_INT, R"(\d+)"},
    {token_group::LITERAL_STRING, R"(\"(?:[^\"\\]|\\.)*\")"},
    {token_group::LITERAL_CHAR, R"('(?:[^'\\]|\\.)')"},

    {token_group::KEYWORD_PACKAGE, R"(\bוישב בארץ\b)"},
    {token_group::KEYWORD_IMPORT_STATIC, R"(\bויבא בכלל\b)"},
    {token_group::KEYWORD_IMPORT, R"(\bויבא\b)"},

    {token_group::SCOPE_BEGIN, R"(\bויאמר\:)"},
    {token_group::SCOPE_END, R"(\bויתם\:)"},

    {token_group::KEYWORD_LET, R"(\bויהי\b)"},
    {token_group::KEYWORD_NAME, R"(\bושמו\b)"},
    {token_group::KEYWORD_SHALL_BE, R"(\b(?:יהיה|תהיה)\b)"},
    {token_group::KEYWORD_RETURN, R"(\bוישב\b)"},

    {token_group::OPERATOR_ADDITION, R"(\bויוסף ל־)"},
    {token_group::OPERATOR_SUBTRACTION, R"(\bויוחסר מן\b)"},
    {token_group::OPERATOR_MULTIPLICATION, R"(\bויוכפל\b)"},
    {token_group::OPERATOR_TIMES, R"(\bפי\b)"},
    {token_group::OPERATOR_DIVISION, R"(\bויחולק\b)"},
    {token_group::OPERATOR_MODULO, R"(\bויושרה\b)"},

    {token_group::OPERATOR_ADDITION_ASSIGNMENT, R"(\bויסף\b)"},
    {token_group::OPERATOR_SUBTRACTION_ASSIGNMENT, R"(\bויחסר\b)"},
    {token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT, R"(\bויכפל\b)"},
    {token_group::OPERATOR_DIVISION_ASSIGNMENT, R"(\bויחלק\b)"},
    {token_group::OPERATOR_MODULO_ASSIGNMENT, R"(\bוישרה\b)"},

    {token_group::OPERATOR_INCREMENT, R"(\bויתקדם\b)"},
    {token_group::OPERATOR_DECREMENT, R"(\bויפחת\b)"},

    {token_group::OPEN_PARENTHESIS, R"(\bכי\b)"},
    {token_group::CLOSE_PARENTHESIS, R"(\bפסק\b)"},

    {token_group::KEYWORD_CLASS, R"(\bעצם\b)"},
    {token_group::KEYWORD_ENUM, R"(\bמניין\b)"},
    {token_group::KEYWORD_INTERFACE, R"(\bצלם\b)"},
    {token_group::KEYWORD_THIS, R"(\bאנכי\b)"},
    {token_group::KEYWORD_IMPLEMENTS, R"(\bוירש את\b)"},
    {token_group::KEYWORD_OF_INSTANCE, R"(\bמזרע\b)"},
    {token_group::KEYWORD_NEW, R"(\bויברא\b)"},

    {token_group::KEYWORD_FUNCTION, R"(\bהמעשה\b)"},
    {token_group::KEYWORD_FUNCTION_CALL, R"(\bויעש\b)"},
    {token_group::KEYWORD_FUNCTION_TAKE_ARGS, R"(\bויקח\b)"},

    {token_group::KEYWORD_PUBLIC, R"(\b(?:נגלה|נגלית)\b)"},
    {token_group::KEYWORD_PRIVATE, R"(\b(?:נסתר|נסתרת)\b)"},
    {token_group::KEYWORD_STATIC, R"(\bכללי\b)"},
    {token_group::KEYWORD_ABSTRACT, R"(\bרוחני\b)"},
    {token_group::KEYWORD_FINAL, R"(\bימות ולא־יתחלף\b)"},
    {token_group::KEYWORD_SUPER, R"(\bאבי\b)"},

    {token_group::KEYWORD_VAR, R"(\bמשתנה\b)"},
    {token_group::KEYWORD_INTEGER, R"(\bשלם\b)"},
    {token_group::KEYWORD_FLOAT, R"(\bצף\b)"},
    {token_group::KEYWORD_DOUBLE, R"(\bמצף\b)"},
    {token_group::KEYWORD_LONG, R"(\bרב\b)"},
    {token_group::KEYWORD_SHORT, R"(\bקצר\b)"},
    {token_group::KEYWORD_BYTE, R"(\bקצרצר\b)"},
    {token_group::KEYWORD_CHAR, R"(\bאות\b)"},
    {token_group::KEYWORD_STRING, R"(\bפסוק\b)"},
    {token_group::KEYWORD_BOOLEAN, R"(\bדבר־מה\b)"},
    {token_group::KEYWORD_NULL, R"(\bתהו\b)"},
    {token_group::KEYWORD_ARRAY, R"(\bכמסדר\b)"},
    {token_group::KEYWORD_INDEX, R"(\bבמקום\b)"},

    {token_group::KEYWORD_REPLACE, R"(\bויחלף\b)"},

    {token_group::KEYWORD_IF, R"(\bאם\b)"},
    {token_group::KEYWORD_ELSE, R"(\bאחרת\b)"},
    {token_group::KEYWORD_SWITCH, R"(\bויחלוק על\b)"},
    {token_group::KEYWORD_CASE, R"(\bאם־כי\b)"},
    {token_group::KEYWORD_DEFAULT, R"(\bבאין צדיק\b)"},

    {token_group::KEYWORD_TRUE, R"(\bאמן\b)"},
    {token_group::KEYWORD_FALSE, R"(\bכזב\b)"},

    {token_group::OPERATOR_EQUALS, R"(\bשווה\b)"},
    {token_group::OPERATOR_NOT_EQUALS, R"(\אינו־שווה\b)"},
    {token_group::OPERATOR_AGAINST, R"(\bלנגד\b)"},
    {token_group::OPERATOR_NOT, R"(\bשקרי־הוא\b)"},
    {token_group::OPERATOR_GREATER, R"(\bגדול\b)"},
    {token_group::OPERATOR_GREATER_EQUAL, R"(\bגדול־ושווה\b)"},
    {token_group::OPERATOR_LESS, R"(\bקטן\b)"},
    {token_group::OPERATOR_LESS_EQUAL, R"(\bקטן־ושווה\b)"},
    {token_group::OPERATOR_AND, R"(\bוגם\b)"},
    {token_group::OPERATOR_OR, R"(\bאו\b)"},


    {token_group::KEYWORD_FOR, R"(\bוימנה\b)"},
    {token_group::KEYWORD_FROM, R"(\bמן\b)"},
    {token_group::KEYWORD_TO, R"(\bעד\b)"},
    {token_group::KEYWORD_SKIP, R"(\bוידלג\b)"},

    {token_group::KEYWORD_DO, R"(\bעשה\b)"},
    {token_group::KEYWORD_WHILE, R"(\bבעוד\b)"},
    {token_group::KEYWORD_CONTINUE, R"(\bפסח\b)"},
    {token_group::KEYWORD_BREAK, R"(\bלך־לך\b)"},


    {token_group::KEYWORD_TRY, R"(\bוינסה\b)"},
    {token_group::KEYWORD_EXCEPTION, R"(\bפסיקה\b)"},
    {token_group::KEYWORD_CATCH, R"(\bויתפוס\b)"},
    {token_group::KEYWORD_THROW, R"(\bוישלך\b)"},
    {token_group::KEYWORD_THROWS, R"(\bוישלכהו\b)"},

    {token_group::PUNCTUATION_END_OP, R"(\:)"},


    {token_group::IDENTIFIER, R"(\w+)"},
};


static std::string build_single_regex(const std::pair<token_group, std::string> &token_regex);


static std::string build_lexer_regex()
{
    if (TOKEN_REGEXES.empty())
        return "";

    std::ostringstream oss;

    oss << build_single_regex(TOKEN_REGEXES[0]);

    for (size_t i = 1; i < TOKEN_REGEXES.size(); ++i)
    {
        oss << '|' << build_single_regex(TOKEN_REGEXES[i]);
    }

    return oss.str();
}

static std::string build_single_regex(const std::pair<token_group, std::string> &token_regex)
{
    std::ostringstream oss;

    oss << '(' << token_regex.second << ')';

    return oss.str();
}


const std::string codesh::LEXER_RGX_STR = build_lexer_regex();
const boost::u32regex codesh::LEXER_RGX = boost::make_u32regex(LEXER_RGX_STR);

/////

token_group codesh::token_group_from_regex_id(const int group_id)
{
    if (group_id > TOKEN_REGEXES.size())
        throw std::invalid_argument("Invalid token group id");

    return TOKEN_REGEXES[group_id - 1].first;
}
