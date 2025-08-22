#pragma once

#include <string_view>

namespace codesh::regex
{

// Group Regexes

constexpr std::string_view NUMBER_RGX = R"(\d+(?:\.\d+)?)";
constexpr std::string_view OPERATOR_RGX = R"([\/\*-\+\%])";
constexpr std::string_view ANYTHING_ELSE_RGX = R"(\S+)";

constexpr std::string_view KEYWORD_IMPORT_RGX = R"(\bויבא\b)";

constexpr std::string_view SCOPE_BEGIN_RGX = R"(\bויאמר\:)";
constexpr std::string_view SCOPE_END_RGX = R"(\bויתם\.)";

constexpr std::string_view KEYWORD_LET_RGX = R"(\bויהי\b)";
constexpr std::string_view KEYWORD_RETURN_RGX = R"(\bוישב\b)";

constexpr std::string_view OPERATOR_ADDITION_RGX = R"(\bויוסף ל־)";
constexpr std::string_view OPERATOR_SUBTRACTION_RGX = R"(\bויוחסר מן\b)";
constexpr std::string_view OPERATOR_MULTIPLICATION_RGX = R"(\bויוכפל\b)";
constexpr std::string_view OPERATOR_DIVISION_RGX = R"(\bויחולק\b)";
constexpr std::string_view OPERATOR_MODULO_RGX = R"(\bויושרה\b)";

constexpr std::string_view OPERATOR_ADDITION_ASSIGNMENT_RGX = R"(\bויסף\b)";
constexpr std::string_view OPERATOR_SUBTRACTION_ASSIGNMENT_RGX = R"(\bויחסר\b)";
constexpr std::string_view OPERATOR_MULTIPLICATION_ASSIGNMENT_RGX = R"(\bויכפל\b)";
constexpr std::string_view OPERATOR_DIVISION_ASSIGNMENT_RGX = R"(\bויחלק\b)";
constexpr std::string_view OPERATOR_MODULO_ASSIGNMENT_RGX = R"(\bוישרה\b)";

constexpr std::string_view OPERATOR_INCREAMENT_RGX = R"(\bויתקדם\b)";
constexpr std::string_view OPERATOR_DECREAMENT_RGX = R"(\bויפחת\b)";

constexpr std::string_view OPEN_PARENTHESIS_RGX = R"(\bכי\b)";
constexpr std::string_view CLOSE_PARENTHESIS_RGX = R"(\bפסק\b)";

constexpr std::string_view SEMICOLON_RGX = R"(\;)";
constexpr std::string_view DOT_RGX = R"(\bל־)";

constexpr std::string_view KEYWORD_CLASS_RGX = R"(\bעצם\b)";
constexpr std::string_view KEYWORD_ENUM_RGX = R"(\bמניין\b)";
constexpr std::string_view KEYWORD_THIS_RGX = R"(\bאנוכי\b)";
constexpr std::string_view KEYWORD_FUNCTION_RGX = R"(\bהמעשה\b)";
constexpr std::string_view KEYWORD_FUNCTION_CALL_RGX = R"(\bויעש\b)";

constexpr std::string_view KEYWORD_PUBLIC_RGX = R"(\b(?:נגלה|נגלית)\b)";
constexpr std::string_view KEYWORD_PRIVATE_RGX = R"(\b(?:נסתר|נסתרת)\b)";
constexpr std::string_view KEYWORD_STATIC_RGX = R"(\bכללי\b)";
constexpr std::string_view KEYWORD_ABSTRACT_RGX = R"(\bרוחני\b)";
constexpr std::string_view KEYWORD_FINAL_RGX = R"(\bוימות ולא־יתחלף\b)";
constexpr std::string_view KEYWORD_SUPER_RGX = R"(\bאבי\b)";

constexpr std::string_view KEYWORD_VAR_RGX = R"(\bמשתנה\b)";
constexpr std::string_view KEYWORD_INTEGER_RGX = R"(\bשלם\b)";
constexpr std::string_view KEYWORD_FLOAT_RGX = R"(\bצף\b)";
constexpr std::string_view KEYWORD_DOUBLE_RGX = R"(\bמצף\b)";
constexpr std::string_view KEYWORD_LONG_RGX = R"(\bרב\b)";
constexpr std::string_view KEYWORD_SHORT_RGX = R"(\bקצר\b)";
constexpr std::string_view KEYWORD_BYTE_RGX = R"(\bקצרצר\b)";
constexpr std::string_view KEYWORD_CHAR_RGX = R"(\bתו\b)";
constexpr std::string_view KEYWORD_STRING_RGX = R"(\bפסוק\b)";
constexpr std::string_view KEYWORD_BOOLEAN_RGX = R"(\bדבר־מה\b)";
constexpr std::string_view KEYWORD_NULL_RGX = R"(\bתהו\b)";

constexpr std::string_view KEYWORD_IF_RGX = R"(\bאם\b)";
constexpr std::string_view KEYWORD_ELSE_RGX = R"(\bאחרת\b)";
constexpr std::string_view KEYWORD_SWITCH_RGX = R"(\bמחלוקת\b)";
constexpr std::string_view KEYWORD_CASE_RGX = R"(\bכאשר\b)";
constexpr std::string_view KEYWORD_DEFAULT_RGX = R"(\bחדל\b)";

constexpr std::string_view OPERATOR_EQUALS_RGX = R"(\bשווה\b)";
constexpr std::string_view OPERATOR_AGAINST_RGX = R"(\bלנגד\b)";
constexpr std::string_view OPERATOR_NOT_RGX = R"(\bשקרי־הוא\b)";
constexpr std::string_view OPERATOR_GREATER_RGX = R"(\bגדול\b)";
constexpr std::string_view OPERATOR_GREATER_EQUAL_RGX = R"(\bגדול־ושווה\b)";
constexpr std::string_view OPERATOR_LESS_RGX = R"(\bקטן\b)";
constexpr std::string_view OPERATOR_LESS_EQUAL_RGX = R"(\bקטן־ושווה\b)";
constexpr std::string_view OPERATOR_AND_RGX = R"(\bוגם\b)";
constexpr std::string_view OPERATOR_OR_RGX = R"(\bאו\b)";

constexpr std::string_view KEYWORD_FOR_RGX = R"(\bוימנה\b)";
constexpr std::string_view KEYWORD_DO_RGX = R"(\bעשה\b)";
constexpr std::string_view KEYWORD_WHILE_RGX = R"(\bבעוד\b)";
constexpr std::string_view KEYWORD_CONTINUE_RGX = R"(\bפסח\b)";
constexpr std::string_view KEYWORD_BREAK_RGX = R"(\bלך־לך\b)";

constexpr std::string_view COMMENT_MULTILINE_RGX = R"(והגה ה\' לאמור:(?:.|\s)*ויחדל.)";
constexpr std::string_view COMMENT_ONE_LINER_RGX = R"(והגה ה\'.*$)";

constexpr std::string_view KEYWORD_TRY_RGX = R"(\bוינסה\b)";
constexpr std::string_view KEYWORD_EXCEPTION_RGX = R"(\bפסיקה\b)";
constexpr std::string_view KEYWORD_CATCH_RGX = R"(\bויתפוס\b)";
constexpr std::string_view KEYWORD_THROW_RGX = R"(\bוישלך\b)";
constexpr std::string_view KEYWORD_THROWS_RGX = R"(\bוישלכהו\b)";

}

