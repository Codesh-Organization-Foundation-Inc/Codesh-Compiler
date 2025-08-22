#ifndef CODESH_COMPILER_REGEX_HELPER_H
#define CODESH_COMPILER_REGEX_HELPER_H

const char* NUMBER_RGX = "\\d+(?:\\.\\d+)?";
const char* OPERATOR_RGX = "[\\/\\*-\\+\\%]";
const char* ANYTHING_ELSE_RGX = "\\S+";

const char* KEYWORD_IMPORT_RGX = "\\bויבא\\b";

const char* SCOPE_BEGIN_RGX ="\\bויאמר\\:";
const char* SCOPE_END_RGX = "\\bויתם\\.";

const char* KEYWORD_LET_RGX = "\\bויהי\\b";
const char* KEYWORD_RETURN_RGX = "\\bוישב\\b";

const char* OPERATOR_ADDITION_RGX = "\\bויוסף ל־";
const char* OPERATOR_SUBTRACTION_RGX = "\\bויוחסר מן\\b";
const char* OPERATOR_MULTIPLICATION_RGX = "\\bויוכפל\\b";
const char* OPERATOR_DIVISION_RGX = "\\bויחולק\\b";
const char* OPERATOR_MODULO_RGX = "\\bויושרה\\b";

const char* OPERATOR_ADDITION_ASSIGNMENT_RGX = "\\bויסף\\b";
const char* OPERATOR_SUBTRACTION_ASSIGNMENT_RGX = "\\bויחסר\\b";
const char* OPERATOR_MULTIPLICATION_ASSIGNMENT_RGX = "\\bויכפל\\b";
const char* OPERATOR_DIVISION_ASSIGNMENT_RGX = "\\bויחלק\\b";
const char* OPERATOR_MODULO_ASSIGNMENT_RGX = "\\bוישרה\\b";

const char* OPERATOR_INCREAMENT_RGX = "\\bויתקדם\\b";
const char* OPERATOR_DECREAMENT_RGX = "\\bויפחת\\b";

const char* OPEN_PARENTHESIS_RGX = "\\bכי\\b";
const char* CLOSE_PARENTHESIS_RGX = "\\bפסק\\b";

const char* SEMICOLON_RGX = "\\;";
const char* DOT_RGX = "\\bל־";

const char* KEYWORD_CLASS_RGX = "\\bעצם\\b";
const char* KEYWORD_ENUM_RGX = "\\bמניין\\b";
const char* KEYWORD_THIS_RGX = "\\bאנוכי\\b";
const char* KEYWORD_FUNCTION_RGX = "\\bהמעשה\\b";
const char* KEYWORD_FUNCTION_CALL_RGX = "\\bויעש\\b";

const char* KEYWORD_PUBLIC_RGX = "\\b(?:נגלה|נגלית)\\b";
const char* KEYWORD_PRIVATE_RGX = "\\b(?:נסתר|נסתרת)\\b";
const char* KEYWORD_STATIC_RGX = "\\bכללי\\b";
const char* KEYWORD_ABSTRACT_RGX = "\\bרוחני\\b";
const char* KEYWORD_FINAL_RGX = "\\bוימות ולא־יתחלף\\b";
const char* KEYWORD_SUPER_RGX = "\\bאבי\\b";

const char* KEYWORD_VAR_RGX = "\\bמשתנה\\b";
const char* KEYWORD_INTEGER_RGX = "\\bשלם\\b";
const char* KEYWORD_FLOAT_RGX = "\\bצף\\b";
const char* KEYWORD_DOUBLE_RGX = "\\bמצף\\b";
const char* KEYWORD_LONG_RGX = "\\bרב\\b";
const char* KEYWORD_SHORT_RGX = "\\bקצר\\b";
const char* KEYWORD_BYTE_RGX = "\\bקצרצר\\b";
const char* KEYWORD_CHAR_RGX = "\\bתו\\b";
const char* KEYWORD_STRING_RGX ="\\bפסוק\\b";
const char* KEYWORD_BOOLEAN_RGX ="\\bדבר־מה\\b";
const char* KEYWORD_NULL_RGX = "\\bתהו\\b";

const char* KEYWORD_IF_RGX = "\\bאם\\b";
const char* KEYWORD_ELSE_RGX = "\\bאחרת\\b";
const char* KEYWORD_SWITCH_RGX = "\\bמחלוקת\\b";
const char* KEYWORD_CASE_RGX = "\\bכאשר\\b";
const char* KEYWORD_DEFAULT_RGX = "\\bחדל\\b";

const char* OPERATOR_EQUALS_RGX = "\\bשווה\\b";
const char* OPERATOR_AGAINST_RGX = "\\bלנגד\\b";
const char* OPERATOR_NOT_RGX = "\\bשקרי־הוא\\b";
const char* OPERATOR_GREATER_RGX = "\\bגדול\\b";
const char* OPERATOR_GREATER_EQUAL_RGX = "\\bגדול־ושווה\\b";
const char* OPERATOR_LESS_RGX = "\\bקטן\\b";
const char* OPERATOR_LESS_EQUAL_RGX = "\\bקטן־ושווה\\b";
const char* OPERATOR_AND_RGX = "\\bוגם\\b";
const char* OPERATOR_OR_RGX = "\\bאו\\b";

const char* KEYWORD_FOR_RGX = "\\bוימנה\\b";
const char* KEYWORD_DO_RGX = "\\bעשה\\b";
const char* KEYWORD_WHILE_RGX = "\\bבעוד\\b";
const char* KEYWORD_CONTINUE_RGX = "\\bפסח\\b";
const char* KEYWORD_BREAK_RGX ="\\bלך־לך\\b";

const char* COMMENT_MULTILINE_RGX = "והגה ה\\' לאמור:(?:.|\\s)*ויחדל.";
const char* COMMENT_ONE_LINER_RGX = "והגה ה\\'.*$";

const char* KEYWORD_TRY_RGX = "\\bוינסה\\b";
const char* KEYWORD_EXCEPTION_RGX = "\\bפסיקה\\b";
const char* KEYWORD_CATCH_RGX = "\\bויתפוס\\b";
const char* KEYWORD_THROW_RGX = "\\bוישלך\\b";
const char* KEYWORD_THROWS_RGX = "\\bוישלכהו\\b";

#endif // CODESH_COMPILER_REGEX_HELPER_H
