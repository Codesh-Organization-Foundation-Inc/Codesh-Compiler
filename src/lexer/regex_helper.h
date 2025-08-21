#ifndef CODESH_COMPILER_REGEX_HELPER_H
#define CODESH_COMPILER_REGEX_HELPER_H

const char* NUMBER_RGX = "\\d+(?:\\.\\d+)?";
const char* OPERATOR_RGX = "[\\/\\*-\\+\\%]";
const char* ANYTHING_ELSE_RGX = "\\S+";

const char* KEYWORD_IMPORT = "\\bויבא\\b";

const char* SCOPE_BEGIN ="\\bויאמר\\:";
const char* SCOPE_END = "\\bויתם\\.";

const char* KEYWORD_LET = "\\bויהי\\b";
const char* KEYWORD_RETURN = "\\bוישב\\b";

const char* OPERATOR_ADDITION = "\\bויוסף ל־";
const char* OPERATOR_SUBTRACTION = "\\bויוחסר מן\\b";
const char* OPERATOR_MULTIPLICATION = "\\bויוכפל\\b";
const char* OPERATOR_DIVISION = "\\bויחולק\\b";
const char* OPERATOR_MODULO = "\\bויושרה\\b";

const char* OPERATOR_ADDITION_ASSIGNMENT = "\\bויסף\\b";
const char* OPERATOR_SUBTRACTION_ASSIGNMENT = "\\bויחסר\\b";
const char* OPERATOR_MULTIPLICATION_ASSIGNMENT = "\\bויכפל\\b";
const char* OPERATOR_DIVISION_ASSIGNMENT = "\\bויחלק\\b";
const char* OPERATOR_MODULO_ASSIGNMENT = "\\bוישרה\\b";

const char* OPERATOR_INCREAMENT = "\\bויתקדם\\b";
const char* OPERATOR_DECREAMENT = "\\bויפחת\\b";

const char* OPEN_PARENTHESIS = "\\bכי\\b";
const char* CLOSE_PARENTHESIS = "\\bפסק\\b";

const char* SEMICOLON = "\\;";
const char* DOT = "\\bל־";

const char* KEYWORD_CLASS = "\\bעצם\\b";
const char* KEYWORD_ENUM = "\\bמניין\\b";
const char* KEYWORD_THIS = "\\bאנוכי\\b";
const char* KEYWORD_FUNCTION = "\\bהמעשה\\b"
const char* KEYWORD_FUNCTION_CALL = "\\bויעש\\b"

const char* KEYWORD_PUBLIC = "\\b(?:נגלה|נגלית)\\b";
const char* KEYWORD_PRIVATE = "\\b(?:נסתר|נסתרת)\\b";
const char* KEYWORD_STATIC = "\\bכללי\\b";
const char* KEYWORD_ABSTRACT = "\\bרוחני\\b";
const char* KEYWORD_FINAL = "\\bוימות ולא־יתחלף\\b";
const char* KEYWORD_SUPER = "\\bאבי\\b";

const char* KEYWORD_VAR = "\\bמשתנה\\b";
const char* KEYWORD_INTEGER = "\\bשלם\\b";
const char* KEYWORD_FLOAT = "\\bצף\\b";
const char* KEYWORD_DOUBLE = "\\bמצף\\b";
const char* KEYWORD_LONG = "\\bרב\\b";
const char* KEYWORD_SHORT = "\\bקצר\\b";
const char* KEYWORD_BYTE = "\\bקצרצר\\b";
const char* KEYWORD_CHAR = "\\bתו\\b";
const char* KEYWORD_STRING ="\\bפסוק\\b";
const char* KEYWORD_BOOLEAN ="\\bדבר־מה\\b";
const char* KEYWORD_NULL = "\\bתהו\\b";

const char* KEYWORD_IF = "\\bאם\\b";
const char* KEYWORD_ELSE = "\\bאחרת\\b";
const char* KEYWORD_SWITCH = "\\bמחלוקת\\b";
const char* KEYWORD_CASE = "\\bכאשר\\b";
const char* KEYWORD_DEFAULT = "\\bחדל\\b";

const char* OPERATOR_EQUALS = "\\bשווה\\b";
const char* OPERATOR_AGAINST = "\\bלנגד\\b";
const char* OPERATOR_NOT = "\\bשקרי־הוא\\b";
const char* OPERATOR_GREATER = "\\bגדול\\b";
const char* OPERATOR_GREATER_EQUAL = "\\bגדול־ושווה\\b";
const char* OPERATOR_LESS = "\\bקטן\\b";
const char* OPERATOR_LESS_EQUAL = "\\bקטן־ושווה\\b";
const char* OPERATOR_AND = "\\bוגם\\b";
const char* OPERATOR_OR = "\\bאו\\b";

const char* KEYWORD_FOR = "\\bוימנה\\b";
const char* KEYWORD_DO = "\\bעשה\\b";
const char* KEYWORD_WHILE = "\\bבעוד\\b";
const char* KEYWORD_CONTINUE = "\\bפסח\\b";
const char* KEYWORD_BREAK ="\\bלך־לך\\b";

const char* COMMENT_MULTILINE = "והגה ה\\' לאמור:(?:.|\\s)*ויחדל.";
const char* COMMENT_ONE_LINER = "והגה ה\\'.*$";

const char* KEYWORD_TRY = "\\bוינסה\\b";
const char* KEYWORD_EXCEPTION = "\\bפסיקה\\b";
const char* KEYWORD_CATCH = "\\bויתפוס\\b";
const char* KEYWORD_THROW = "\\bוישלך\\b";
const char* KEYWORD_THROWS = "\\bוישלכהו\\b";



#endif // CODESH_COMPILER_REGEX_HELPER_H
