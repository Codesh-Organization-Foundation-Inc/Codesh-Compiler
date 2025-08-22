#pragma once


// Group Regexes

const char *const NUMBER_RGX = "\\d+(?:\\.\\d+)?";
const char *const OPERATOR_RGX = "[\\/\\*-\\+\\%]";
const char *const ANYTHING_ELSE_RGX = "\\S+";

const char *const KEYWORD_IMPORT_RGX = "\\bויבא\\b";

const char *const SCOPE_BEGIN_RGX = "\\bויאמר\\:";
const char *const SCOPE_END_RGX = "\\bויתם\\.";

const char *const KEYWORD_LET_RGX = "\\bויהי\\b";
const char *const KEYWORD_RETURN_RGX = "\\bוישב\\b";

const char *const OPERATOR_ADDITION_RGX = "\\bויוסף ל־";
const char *const OPERATOR_SUBTRACTION_RGX = "\\bויוחסר מן\\b";
const char *const OPERATOR_MULTIPLICATION_RGX = "\\bויוכפל\\b";
const char *const OPERATOR_DIVISION_RGX = "\\bויחולק\\b";
const char *const OPERATOR_MODULO_RGX = "\\bויושרה\\b";

const char *const OPERATOR_ADDITION_ASSIGNMENT_RGX = "\\bויסף\\b";
const char *const OPERATOR_SUBTRACTION_ASSIGNMENT_RGX = "\\bויחסר\\b";
const char *const OPERATOR_MULTIPLICATION_ASSIGNMENT_RGX = "\\bויכפל\\b";
const char *const OPERATOR_DIVISION_ASSIGNMENT_RGX = "\\bויחלק\\b";
const char *const OPERATOR_MODULO_ASSIGNMENT_RGX = "\\bוישרה\\b";

const char *const OPERATOR_INCREAMENT_RGX = "\\bויתקדם\\b";
const char *const OPERATOR_DECREAMENT_RGX = "\\bויפחת\\b";

const char *const OPEN_PARENTHESIS_RGX = "\\bכי\\b";
const char *const CLOSE_PARENTHESIS_RGX = "\\bפסק\\b";

const char *const SEMICOLON_RGX = "\\;";
const char *const DOT_RGX = "\\bל־";

const char *const KEYWORD_CLASS_RGX = "\\bעצם\\b";
const char *const KEYWORD_ENUM_RGX = "\\bמניין\\b";
const char *const KEYWORD_THIS_RGX = "\\bאנוכי\\b";
const char *const KEYWORD_FUNCTION_RGX = "\\bהמעשה\\b";
const char *const KEYWORD_FUNCTION_CALL_RGX = "\\bויעש\\b";

const char *const KEYWORD_PUBLIC_RGX = "\\b(?:נגלה|נגלית)\\b";
const char *const KEYWORD_PRIVATE_RGX = "\\b(?:נסתר|נסתרת)\\b";
const char *const KEYWORD_STATIC_RGX = "\\bכללי\\b";
const char *const KEYWORD_ABSTRACT_RGX = "\\bרוחני\\b";
const char *const KEYWORD_FINAL_RGX = "\\bוימות ולא־יתחלף\\b";
const char *const KEYWORD_SUPER_RGX = "\\bאבי\\b";

const char *const KEYWORD_VAR_RGX = "\\bמשתנה\\b";
const char *const KEYWORD_INTEGER_RGX = "\\bשלם\\b";
const char *const KEYWORD_FLOAT_RGX = "\\bצף\\b";
const char *const KEYWORD_DOUBLE_RGX = "\\bמצף\\b";
const char *const KEYWORD_LONG_RGX = "\\bרב\\b";
const char *const KEYWORD_SHORT_RGX = "\\bקצר\\b";
const char *const KEYWORD_BYTE_RGX = "\\bקצרצר\\b";
const char *const KEYWORD_CHAR_RGX = "\\bתו\\b";
const char *const KEYWORD_STRING_RGX = "\\bפסוק\\b";
const char *const KEYWORD_BOOLEAN_RGX = "\\bדבר־מה\\b";
const char *const KEYWORD_NULL_RGX = "\\bתהו\\b";

const char *const KEYWORD_IF_RGX = "\\bאם\\b";
const char *const KEYWORD_ELSE_RGX = "\\bאחרת\\b";
const char *const KEYWORD_SWITCH_RGX = "\\bמחלוקת\\b";
const char *const KEYWORD_CASE_RGX = "\\bכאשר\\b";
const char *const KEYWORD_DEFAULT_RGX = "\\bחדל\\b";

const char *const OPERATOR_EQUALS_RGX = "\\bשווה\\b";
const char *const OPERATOR_AGAINST_RGX = "\\bלנגד\\b";
const char *const OPERATOR_NOT_RGX = "\\bשקרי־הוא\\b";
const char *const OPERATOR_GREATER_RGX = "\\bגדול\\b";
const char *const OPERATOR_GREATER_EQUAL_RGX = "\\bגדול־ושווה\\b";
const char *const OPERATOR_LESS_RGX = "\\bקטן\\b";
const char *const OPERATOR_LESS_EQUAL_RGX = "\\bקטן־ושווה\\b";
const char *const OPERATOR_AND_RGX = "\\bוגם\\b";
const char *const OPERATOR_OR_RGX = "\\bאו\\b";

const char *const KEYWORD_FOR_RGX = "\\bוימנה\\b";
const char *const KEYWORD_DO_RGX = "\\bעשה\\b";
const char *const KEYWORD_WHILE_RGX = "\\bבעוד\\b";
const char *const KEYWORD_CONTINUE_RGX = "\\bפסח\\b";
const char *const KEYWORD_BREAK_RGX = "\\bלך־לך\\b";

const char *const COMMENT_MULTILINE_RGX = "והגה ה\\' לאמור:(?:.|\\s)*ויחדל.";
const char *const COMMENT_ONE_LINER_RGX = "והגה ה\\'.*$";

const char *const KEYWORD_TRY_RGX = "\\bוינסה\\b";
const char *const KEYWORD_EXCEPTION_RGX = "\\bפסיקה\\b";
const char *const KEYWORD_CATCH_RGX = "\\bויתפוס\\b";
const char *const KEYWORD_THROW_RGX = "\\bוישלך\\b";
const char *const KEYWORD_THROWS_RGX = "\\bוישלכהו\\b";

// Group Names

struct group_name_to_regex
{
    const char *const group_name;
    const char *const regex;
};

const struct group_name_to_regex CODESH_GN2R[] = {
    {"NUMBER", NUMBER_RGX},
    {"OPERATOR", OPERATOR_RGX},
    {"ANYTHING_ELSE", ANYTHING_ELSE_RGX},
    {"KEYWORD_IMPORT", KEYWORD_IMPORT_RGX},
    {"SCOPE_END", SCOPE_END_RGX},
    {"KEYWORD_LET", KEYWORD_LET_RGX},
    {"KEYWORD_RETURN", KEYWORD_RETURN_RGX},
    {"OPERATOR_ADDITION", OPERATOR_ADDITION_RGX},
    {"OPERATOR_SUBTRACTION", OPERATOR_SUBTRACTION_RGX},
    {"OPERATOR_MULTIPLICATION", OPERATOR_MULTIPLICATION_RGX},
    {"OPERATOR_DIVISION", OPERATOR_DIVISION_RGX},
    {"OPERATOR_MODULO", OPERATOR_MODULO_RGX},
    {"OPERATOR_ADDITION_ASSIGNMENT", OPERATOR_ADDITION_ASSIGNMENT_RGX},
    {"OPERATOR_SUBTRACTION_ASSIGNMENT", OPERATOR_SUBTRACTION_ASSIGNMENT_RGX},
    {"OPERATOR_MULTIPLICATION_ASSIGNMENT", OPERATOR_MULTIPLICATION_ASSIGNMENT_RGX},
    {"OPERATOR_DIVISION_ASSIGNMENT", OPERATOR_DIVISION_ASSIGNMENT_RGX},
    {"OPERATOR_MODULO_ASSIGNMENT", OPERATOR_MODULO_ASSIGNMENT_RGX},
    {"OPERATOR_INCREAMENT", OPERATOR_INCREAMENT_RGX},
    {"OPERATOR_DECREAMENT", OPERATOR_DECREAMENT_RGX},
    {"OPEN_PARENTHESIS", OPEN_PARENTHESIS_RGX},
    {"CLOSE_PARENTHESIS", CLOSE_PARENTHESIS_RGX},
    {"SEMICOLON", SEMICOLON_RGX},
    {"DOT", DOT_RGX},
    {"KEYWORD_CLASS", KEYWORD_CLASS_RGX},
    {"KEYWORD_ENUM", KEYWORD_ENUM_RGX},
    {"KEYWORD_THIS", KEYWORD_THIS_RGX},
    {"KEYWORD_FUNCTION", KEYWORD_FUNCTION_RGX},
    {"KEYWORD_FUNCTION_CALL", KEYWORD_FUNCTION_CALL_RGX},
    {"KEYWORD_PUBLIC", KEYWORD_PUBLIC_RGX},
    {"KEYWORD_PRIVATE", KEYWORD_PRIVATE_RGX},
    {"KEYWORD_STATIC", KEYWORD_STATIC_RGX},
    {"KEYWORD_ABSTRACT", KEYWORD_ABSTRACT_RGX},
    {"KEYWORD_FINAL", KEYWORD_FINAL_RGX},
    {"KEYWORD_SUPER", KEYWORD_SUPER_RGX},
    {"KEYWORD_VAR", KEYWORD_VAR_RGX},
    {"KEYWORD_INTEGER", KEYWORD_INTEGER_RGX},
    {"KEYWORD_FLOAT", KEYWORD_FLOAT_RGX},
    {"KEYWORD_DOUBLE", KEYWORD_DOUBLE_RGX},
    {"KEYWORD_LONG", KEYWORD_LONG_RGX},
    {"KEYWORD_SHORT", KEYWORD_SHORT_RGX},
    {"KEYWORD_BYTE", KEYWORD_BYTE_RGX},
    {"KEYWORD_CHAR", KEYWORD_CHAR_RGX},
    {"KEYWORD_NULL", KEYWORD_NULL_RGX},
    {"KEYWORD_IF", KEYWORD_IF_RGX},
    {"KEYWORD_ELSE", KEYWORD_ELSE_RGX},
    {"KEYWORD_SWITCH", KEYWORD_SWITCH_RGX},
    {"KEYWORD_CASE", KEYWORD_CASE_RGX},
    {"KEYWORD_DEFAULT", KEYWORD_DEFAULT_RGX},
    {"OPERATOR_EQUALS", OPERATOR_EQUALS_RGX},
    {"OPERATOR_AGAINST", OPERATOR_AGAINST_RGX},
    {"OPERATOR_NOT", OPERATOR_NOT_RGX},
    {"OPERATOR_GREATER", OPERATOR_GREATER_RGX},
    {"OPERATOR_GREATER_EQUAL", OPERATOR_GREATER_EQUAL_RGX},
    {"OPERATOR_LESS", OPERATOR_LESS_RGX},
    {"OPERATOR_LESS_EQUAL", OPERATOR_LESS_EQUAL_RGX},
    {"OPERATOR_AND", OPERATOR_AND_RGX},
    {"OPERATOR_OR", OPERATOR_OR_RGX},
    {"KEYWORD_FOR", KEYWORD_FOR_RGX},
    {"KEYWORD_DO", KEYWORD_DO_RGX},
    {"KEYWORD_WHILE", KEYWORD_WHILE_RGX},
    {"KEYWORD_CONTINUE", KEYWORD_CONTINUE_RGX},
    {"COMMENT_MULTILINE", COMMENT_MULTILINE_RGX},
    {"COMMENT_ONE_LINER", COMMENT_ONE_LINER_RGX},
    {"KEYWORD_TRY", KEYWORD_TRY_RGX},
    {"KEYWORD_EXCEPTION", KEYWORD_EXCEPTION_RGX},
    {"KEYWORD_CATCH", KEYWORD_CATCH_RGX},
    {"KEYWORD_THROW", KEYWORD_THROW_RGX},
    {"KEYWORD_THROWS", KEYWORD_THROWS_RGX},
};
