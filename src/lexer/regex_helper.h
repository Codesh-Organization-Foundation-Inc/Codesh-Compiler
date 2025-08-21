#ifndef CODESH_COMPILER_REGEX_HELPER_H
#define CODESH_COMPILER_REGEX_HELPER_H

const char* NUMBER_RGX = "\\d+(?:\\.\\d+)?";
const char* OPERATOR_RGX = "[\\/\\*-\\+\\%]";
const char* RIGHT_PARA_RGX = "\\(";
const char* LEFT_PARA_RGX = "\\)";
const char* ANYTHING_ELSE_RGX = "\\w+";

const char* SCOPE_BEGIN ="ויאמר\\:";
const char* SCOPE_END = "ויתם\\.";

const char* KEYWORD_LET = "ויהי";
const char* KEYWORD_RETURN = "וישב";

const char* OPERATOR_ADDITION = "ויוסף ל־";
const char* OPERATOR_SUBTRACTION = "ויוחסר מן";
const char* OPERATOR_MULTIPLICATION = "ויוכפל";
const char* OPERATOR_DIVISION = "ויחולק";
const char* OPERATOR_MODULO = "ויושרה";

const char* OPERATOR_ADDITION_ASSIGNMENT = "ויסף";
const char* OPERATOR_SUBTRACTION_ASSIGNMENT = "ויחסר";
const char* OPERATOR_MULTIPLICATION_ASSIGNMENT = "ויכפל";
const char* OPERATOR_DIVISION_ASSIGNMENT = "ויחלק";
const char* OPERATOR_MODULO_ASSIGNMENT = "וישרה";

const char* OPERATOR_INCREAMENT = "ויתקדם";
const char* OPERATOR_DECREAMENT = "ויפחת";

const char* OPEN_PARENTHESIS = "כי";
const char* CLOSE_PARENTHESIS = "פסק";

const char* SEMICOLON = "\\;";
const char* DOT = "\\bל־";

const char* CLASS = "עצם";
const char* ENUM = "מניין";
const char* THIS = "אנוכי";
const char* FUNCTION = "המעשה"
const char* FUNCTION_CALL = "ויעש"

const char* KEYWORD_PUBLIC = "נגלה|נגלית";
const char* KEYWORD_PRIVATE = "נסתר|נסתרת";
const char* KEYWORD_STATIC = "כללי";
const char* KEYWORD_ABSTRACT = "רוחני";
const char* KEYWORD_FINAL = "וימות ולא־יתחלף";
const char* KEYWORD_SUPER = "אבי";

const char* VAR = "משתנה";
const char* INTEGER = "שלם";
const char* FLOAT = "צף";
const char* DOUBLE = "מצף";
const char* LONG = "רב";
const char* SHORT = "קצר";
const char* BYTE = "קצרצר";
const char* CHAR = "תו";
const char* STRING ="פסוק";
const char* BOOLEAN ="דבר־מה";
const char* NULL = "תהו";

const char* KEYWORD_IF = "אם";
const char* KEYWORD_ELSE = "אחרת";
const char* KEYWORD_SWITCH = "מחלוקת";
const char* KEYWORD_CASE = "כאשר";
const char* KEYWORD_DEFAULT = "חדל";

const char* OPERATOR_EQUALS = "שווה";
const char* OPERATOR_AGAINST = "לנגד";
const char* KEYWORD_NOT = "שקרי־הוא";
const char* KEYWORD_GREATER = "גדול";
const char* KEYWORD_GREATER_EQUAL = "גדול־ושווה";
const char* KEYWORD_LESS = "קטן";
const char* KEYWORD_LESS_EQUAL = "קטן־ושווה";
const char* KEYWORD_AND = "וגם";
const char* KEYWORD_OR = "או";

const char* KEYWORD_FOR = "וימנה";
const char* KEYWORD_DO = "עשה";
const char* KEYWORD_WHILE = "בעוד";
const char* KEYWORD_CONTINUE = "פסח";
const char* KEYWORD_BREAK ="לך־לך";

const char* COMMENT_ONE_LINER = "והגה ה\\'";
const char* COMMENT_MULTILINE = "והגה ה\\' לאמור\\:";

const char* KEYWORD_TRY = "וינסה";
const char* KEYWORD_EXCEPTION = "פסיקה";
const char* KEYWORD_CATCH = "ויתפוס";
const char* KEYWORD_THROW = "וישלך";
const char* KEYWORD_THROWS = "וישלכהו";

const char* KEYWORD_IMPORT = "ויבא";


#endif // CODESH_COMPILER_REGEX_HELPER_H
