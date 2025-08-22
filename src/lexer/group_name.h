#pragma once
#include <string_view>

namespace codesh::regex::group_name
{

constexpr std::string_view IDENTIFIER_NUMBER_GROUP_NAME = "IDENTIFIER_NUMBER";
constexpr std::string_view IDENTIFIER_CUSTOM_GROUP_NAME = "IDENTIFIER_CUSTOM";

constexpr std::string_view KEYWORD_IMPORT_GROUP_NAME = "KEYWORD_IMPORT";

constexpr std::string_view SCOPE_BEGIN_GROUP_NAME = "SCOPE_BEGIN";
constexpr std::string_view SCOPE_END_GROUP_NAME = "SCOPE_END";

constexpr std::string_view KEYWORD_LET_GROUP_NAME = "KEYWORD_LET";
constexpr std::string_view KEYWORD_NAME_GROUP_NAME = "KEYWORD_NAME";
constexpr std::string_view KEYWORD_SHALL_BE_GROUP_NAME = "KEYWORD_SHALL_BE";
constexpr std::string_view KEYWORD_RETURN_GROUP_NAME = "KEYWORD_RETURN";

constexpr std::string_view OPERATOR_ADDITION_GROUP_NAME = "OPERATOR_ADDITION";
constexpr std::string_view OPERATOR_SUBTRACTION_GROUP_NAME = "OPERATOR_SUBTRACTION";
constexpr std::string_view OPERATOR_MULTIPLICATION_GROUP_NAME = "OPERATOR_MULTIPLICATION";
constexpr std::string_view OPERATOR_DIVISION_GROUP_NAME = "OPERATOR_DIVISION";
constexpr std::string_view OPERATOR_MODULO_GROUP_NAME = "OPERATOR_MODULO";

constexpr std::string_view OPERATOR_ADDITION_ASSIGNMENT_GROUP_NAME = "OPERATOR_ADDITION_ASSIGNMENT";
constexpr std::string_view OPERATOR_SUBTRACTION_ASSIGNMENT_GROUP_NAME = "OPERATOR_SUBTRACTION_ASSIGNMENT";
constexpr std::string_view OPERATOR_MULTIPLICATION_ASSIGNMENT_GROUP_NAME = "OPERATOR_MULTIPLICATION_ASSIGNMENT";
constexpr std::string_view OPERATOR_DIVISION_ASSIGNMENT_GROUP_NAME = "OPERATOR_DIVISION_ASSIGNMENT";
constexpr std::string_view OPERATOR_MODULO_ASSIGNMENT_GROUP_NAME = "OPERATOR_MODULO_ASSIGNMENT";

constexpr std::string_view OPERATOR_INCREMENT_GROUP_NAME = "OPERATOR_INCREMENT";
constexpr std::string_view OPERATOR_DECREMENT_GROUP_NAME = "OPERATOR_DECREMENT";

constexpr std::string_view OPEN_PARENTHESIS_GROUP_NAME = "OPEN_PARENTHESIS";
constexpr std::string_view CLOSE_PARENTHESIS_GROUP_NAME = "CLOSE_PARENTHESIS";

constexpr std::string_view PUNCTUATION_COMMA_GROUP_NAME = "COMMA";
constexpr std::string_view PUNCTUATION_SEMICOLON_GROUP_NAME = "SEMICOLON";
constexpr std::string_view PUNCTUATION_DOT_GROUP_NAME = "DOT";

constexpr std::string_view KEYWORD_CLASS_GROUP_NAME = "KEYWORD_CLASS";
constexpr std::string_view KEYWORD_ENUM_GROUP_NAME = "KEYWORD_ENUM";
constexpr std::string_view KEYWORD_THIS_GROUP_NAME = "KEYWORD_THIS";
constexpr std::string_view KEYWORD_FUNCTION_GROUP_NAME = "KEYWORD_FUNCTION";
constexpr std::string_view KEYWORD_FUNCTION_CALL_GROUP_NAME = "KEYWORD_FUNCTION_CALL";

constexpr std::string_view KEYWORD_PUBLIC_GROUP_NAME = "KEYWORD_PUBLIC";
constexpr std::string_view KEYWORD_PRIVATE_GROUP_NAME = "KEYWORD_PRIVATE";
constexpr std::string_view KEYWORD_STATIC_GROUP_NAME = "KEYWORD_STATIC";
constexpr std::string_view KEYWORD_ABSTRACT_GROUP_NAME = "KEYWORD_ABSTRACT";
constexpr std::string_view KEYWORD_FINAL_GROUP_NAME = "KEYWORD_FINAL";
constexpr std::string_view KEYWORD_SUPER_GROUP_NAME = "KEYWORD_SUPER";

constexpr std::string_view KEYWORD_VAR_GROUP_NAME = "KEYWORD_VAR";
constexpr std::string_view KEYWORD_INTEGER_GROUP_NAME = "KEYWORD_INTEGER";
constexpr std::string_view KEYWORD_FLOAT_GROUP_NAME = "KEYWORD_FLOAT";
constexpr std::string_view KEYWORD_DOUBLE_GROUP_NAME = "KEYWORD_DOUBLE";
constexpr std::string_view KEYWORD_LONG_GROUP_NAME = "KEYWORD_LONG";
constexpr std::string_view KEYWORD_SHORT_GROUP_NAME = "KEYWORD_SHORT";
constexpr std::string_view KEYWORD_BYTE_GROUP_NAME = "KEYWORD_BYTE";
constexpr std::string_view KEYWORD_CHAR_GROUP_NAME = "KEYWORD_CHAR";
constexpr std::string_view KEYWORD_STRING_GROUP_NAME = "KEYWORD_STRING";
constexpr std::string_view KEYWORD_BOOLEAN_GROUP_NAME = "KEYWORD_BOOLEAN";
constexpr std::string_view KEYWORD_NULL_GROUP_NAME = "KEYWORD_NULL";

constexpr std::string_view KEYWORD_IF_GROUP_NAME = "KEYWORD_IF";
constexpr std::string_view KEYWORD_ELSE_GROUP_NAME = "KEYWORD_ELSE";
constexpr std::string_view KEYWORD_SWITCH_GROUP_NAME = "KEYWORD_SWITCH";
constexpr std::string_view KEYWORD_CASE_GROUP_NAME = "KEYWORD_CASE";
constexpr std::string_view KEYWORD_DEFAULT_GROUP_NAME = "KEYWORD_DEFAULT";

constexpr std::string_view OPERATOR_EQUALS_GROUP_NAME = "OPERATOR_EQUALS";
constexpr std::string_view OPERATOR_AGAINST_GROUP_NAME = "OPERATOR_AGAINST";
constexpr std::string_view OPERATOR_NOT_GROUP_NAME = "OPERATOR_NOT";
constexpr std::string_view OPERATOR_GREATER_GROUP_NAME = "OPERATOR_GREATER";
constexpr std::string_view OPERATOR_GREATER_EQUAL_GROUP_NAME = "OPERATOR_GREATER_EQUAL";
constexpr std::string_view OPERATOR_LESS_GROUP_NAME = "OPERATOR_LESS";
constexpr std::string_view OPERATOR_LESS_EQUAL_GROUP_NAME = "OPERATOR_LESS_EQUAL";
constexpr std::string_view OPERATOR_AND_GROUP_NAME = "OPERATOR_AND";
constexpr std::string_view OPERATOR_OR_GROUP_NAME = "OPERATOR_OR";

constexpr std::string_view KEYWORD_FOR_GROUP_NAME = "KEYWORD_FOR";
constexpr std::string_view KEYWORD_DO_GROUP_NAME = "KEYWORD_DO";
constexpr std::string_view KEYWORD_WHILE_GROUP_NAME = "KEYWORD_WHILE";
constexpr std::string_view KEYWORD_CONTINUE_GROUP_NAME = "KEYWORD_CONTINUE";
constexpr std::string_view KEYWORD_BREAK_GROUP_NAME = "KEYWORD_BREAK";

constexpr std::string_view COMMENT_MULTILINE_GROUP_NAME = "COMMENT_MULTILINE";
constexpr std::string_view COMMENT_ONE_LINER_GROUP_NAME = "COMMENT_ONE_LINER";

constexpr std::string_view KEYWORD_TRY_GROUP_NAME = "KEYWORD_TRY";
constexpr std::string_view KEYWORD_EXCEPTION_GROUP_NAME = "KEYWORD_EXCEPTION";
constexpr std::string_view KEYWORD_CATCH_GROUP_NAME = "KEYWORD_CATCH";
constexpr std::string_view KEYWORD_THROW_GROUP_NAME = "KEYWORD_THROW";
constexpr std::string_view KEYWORD_THROWS_GROUP_NAME = "KEYWORD_THROWS";

}