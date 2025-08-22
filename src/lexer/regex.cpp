#include "regex.h"

#include "group_name.h"

#include <format>
#include <sstream>
#include <vector>

using namespace codesh::regex::group_name;


// The order of this vector determines the order of regex priority.
static const std::vector<std::pair<std::string_view, std::string>> TOKEN_REGEXES = {
    {COMMENT_MULTILINE_GROUP_NAME, R"(והגה ה\' לאמור:(?:.|\s)*ויחדל.)"},
    {COMMENT_ONE_LINER_GROUP_NAME, R"(והגה ה\'.*$)"},

    {COMMA_GROUP_NAME, R"(\,)"},
    {SEMICOLON_GROUP_NAME, R"(\;)"},
    {DOT_GROUP_NAME, R"(\bל־)"},

    {IDENTIFIER_NUMBER_GROUP_NAME, R"(\d+(?:\.\d+)?)"},

    {KEYWORD_IMPORT_GROUP_NAME, R"(\bויבא\b)"},

    {SCOPE_BEGIN_GROUP_NAME, R"(\bויאמר\:)"},
    {SCOPE_END_GROUP_NAME, R"(\bויתם\.)"},

    {KEYWORD_LET_GROUP_NAME, R"(\bויהי\b)"},
    {KEYWORD_NAME_GROUP_NAME, R"(\bושמו\b)"},
    {KEYWORD_SHALL_BE_GROUP_NAME, R"(\b(?:יהיה|תהיה)\b)"},
    {KEYWORD_RETURN_GROUP_NAME, R"(\bוישב\b)"},

    {OPERATOR_ADDITION_GROUP_NAME, R"(\bויוסף ל־)"},
    {OPERATOR_SUBTRACTION_GROUP_NAME, R"(\bויוחסר מן\b)"},
    {OPERATOR_MULTIPLICATION_GROUP_NAME, R"(\bויוכפל\b)"},
    {OPERATOR_DIVISION_GROUP_NAME, R"(\bויחולק\b)"},
    {OPERATOR_MODULO_GROUP_NAME, R"(\bויושרה\b)"},

    {OPERATOR_ADDITION_ASSIGNMENT_GROUP_NAME, R"(\bויסף\b)"},
    {OPERATOR_SUBTRACTION_ASSIGNMENT_GROUP_NAME, R"(\bויחסר\b)"},
    {OPERATOR_MULTIPLICATION_ASSIGNMENT_GROUP_NAME, R"(\bויכפל\b)"},
    {OPERATOR_DIVISION_ASSIGNMENT_GROUP_NAME, R"(\bויחלק\b)"},
    {OPERATOR_MODULO_ASSIGNMENT_GROUP_NAME, R"(\bוישרה\b)"},

    {OPERATOR_INCREMENT_GROUP_NAME, R"(\bויתקדם\b)"},
    {OPERATOR_DECREMENT_GROUP_NAME, R"(\bויפחת\b)"},

    {OPEN_PARENTHESIS_GROUP_NAME, R"(\bכי\b)"},
    {CLOSE_PARENTHESIS_GROUP_NAME, R"(\bפסק\b)"},

    {KEYWORD_CLASS_GROUP_NAME, R"(\bעצם\b)"},
    {KEYWORD_ENUM_GROUP_NAME, R"(\bמניין\b)"},
    {KEYWORD_THIS_GROUP_NAME, R"(\bאנוכי\b)"},
    {KEYWORD_FUNCTION_GROUP_NAME, R"(\bהמעשה\b)"},
    {KEYWORD_FUNCTION_CALL_GROUP_NAME, R"(\bויעש\b)"},

    {KEYWORD_PUBLIC_GROUP_NAME, R"(\b(?:נגלה|נגלית)\b)"},
    {KEYWORD_PRIVATE_GROUP_NAME, R"(\b(?:נסתר|נסתרת)\b)"},
    {KEYWORD_STATIC_GROUP_NAME, R"(\bכללי\b)"},
    {KEYWORD_ABSTRACT_GROUP_NAME, R"(\bרוחני\b)"},
    {KEYWORD_FINAL_GROUP_NAME, R"(\bוימות ולא־יתחלף\b)"},
    {KEYWORD_SUPER_GROUP_NAME, R"(\bאבי\b)"},

    {KEYWORD_VAR_GROUP_NAME, R"(\bמשתנה\b)"},
    {KEYWORD_INTEGER_GROUP_NAME, R"(\bשלם\b)"},
    {KEYWORD_FLOAT_GROUP_NAME, R"(\bצף\b)"},
    {KEYWORD_DOUBLE_GROUP_NAME, R"(\bמצף\b)"},
    {KEYWORD_LONG_GROUP_NAME, R"(\bרב\b)"},
    {KEYWORD_SHORT_GROUP_NAME, R"(\bקצר\b)"},
    {KEYWORD_BYTE_GROUP_NAME, R"(\bקצרצר\b)"},
    {KEYWORD_CHAR_GROUP_NAME, R"(\bתו\b)"},
    {KEYWORD_STRING_GROUP_NAME, R"(\bפסוק\b)"},
    {KEYWORD_BOOLEAN_GROUP_NAME, R"(\bדבר־מה\b)"},
    {KEYWORD_NULL_GROUP_NAME, R"(\bתהו\b)"},

    {KEYWORD_IF_GROUP_NAME, R"(\bאם\b)"},
    {KEYWORD_ELSE_GROUP_NAME, R"(\bאחרת\b)"},
    {KEYWORD_SWITCH_GROUP_NAME, R"(\bמחלוקת\b)"},
    {KEYWORD_CASE_GROUP_NAME, R"(\bכאשר\b)"},
    {KEYWORD_DEFAULT_GROUP_NAME, R"(\bחדל\b)"},

    {OPERATOR_EQUALS_GROUP_NAME, R"(\bשווה\b)"},
    {OPERATOR_AGAINST_GROUP_NAME, R"(\bלנגד\b)"},
    {OPERATOR_NOT_GROUP_NAME, R"(\bשקרי־הוא\b)"},
    {OPERATOR_GREATER_GROUP_NAME, R"(\bגדול\b)"},
    {OPERATOR_GREATER_EQUAL_GROUP_NAME, R"(\bגדול־ושווה\b)"},
    {OPERATOR_LESS_GROUP_NAME, R"(\bקטן\b)"},
    {OPERATOR_LESS_EQUAL_GROUP_NAME, R"(\bקטן־ושווה\b)"},
    {OPERATOR_AND_GROUP_NAME, R"(\bוגם\b)"},
    {OPERATOR_OR_GROUP_NAME, R"(\bאו\b)"},

    {KEYWORD_FOR_GROUP_NAME, R"(\bוימנה\b)"},
    {KEYWORD_DO_GROUP_NAME, R"(\bעשה\b)"},
    {KEYWORD_WHILE_GROUP_NAME, R"(\bבעוד\b)"},
    {KEYWORD_CONTINUE_GROUP_NAME, R"(\bפסח\b)"},
    {KEYWORD_BREAK_GROUP_NAME, R"(\bלך־לך\b)"},

    {KEYWORD_TRY_GROUP_NAME, R"(\bוינסה\b)"},
    {KEYWORD_EXCEPTION_GROUP_NAME, R"(\bפסיקה\b)"},
    {KEYWORD_CATCH_GROUP_NAME, R"(\bויתפוס\b)"},
    {KEYWORD_THROW_GROUP_NAME, R"(\bוישלך\b)"},
    {KEYWORD_THROWS_GROUP_NAME, R"(\bוישלכהו\b)"},


    {IDENTIFIER_CUSTOM_GROUP_NAME, R"(\S+)"},
};


static std::string build_single_regex(const std::pair<std::string_view, std::string_view>& token_regex);


static std::string build_lexer_regex()
{
    if (TOKEN_REGEXES.empty())
        return "";

    std::ostringstream oss;

    oss << build_single_regex(TOKEN_REGEXES[0]);

    for (size_t i = 1; i < TOKEN_REGEXES.size(); ++i)
    {
        oss << "|" << build_single_regex(TOKEN_REGEXES[i]);
    }

    return oss.str();
}

static std::string build_single_regex(const std::pair<std::string_view, std::string_view>& token_regex)
{
    std::ostringstream oss;

    oss << "(?<" << token_regex.first << ">" << token_regex.second << ")";

    return oss.str();
}


const std::string codesh::LEXER_RGX = build_lexer_regex();
