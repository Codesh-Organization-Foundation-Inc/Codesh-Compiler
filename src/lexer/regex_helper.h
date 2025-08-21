#ifndef CODESH_COMPILER_REGEX_HELPER_H
#define CODESH_COMPILER_REGEX_HELPER_H

const char* NUMBER_RGX = "\\d+(?:\\.\\d+)?";
const char* OPERATOR_RGX = "[\\/\\*-\\+\\%]";
const char* ANYTHING_ELSE_RGX = "\\S+";

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

const char* KEYWORD_CLASS = "עצם";
const char* KEYWORD_ENUM = "מניין";
const char* KEYWORD_THIS = "אנוכי";
const char* KEYWORD_FUNCTION = "המעשה"
const char* KEYWORD_FUNCTION_CALL = "ויעש"

const char* KEYWORD_PUBLIC = "נגלה|נגלית";
const char* KEYWORD_PRIVATE = "נסתר|נסתרת";
const char* KEYWORD_STATIC = "כללי";
const char* KEYWORD_ABSTRACT = "רוחני";
const char* KEYWORD_FINAL = "וימות ולא־יתחלף";
const char* KEYWORD_SUPER = "אבי";

const char* KEYWORD_VAR = "משתנה";
const char* KEYWORD_INTEGER = "שלם";
const char* KEYWORD_FLOAT = "צף";
const char* KEYWORD_DOUBLE = "מצף";
const char* KEYWORD_LONG = "רב";
const char* KEYWORD_SHORT = "קצר";
const char* KEYWORD_BYTE = "קצרצר";
const char* KEYWORD_CHAR = "תו";
const char* KEYWORD_STRING ="פסוק";
const char* KEYWORD_BOOLEAN ="דבר־מה";
const char* KEYWORD_NULL = "תהו";

const char* KEYWORD_IF = "אם";
const char* KEYWORD_ELSE = "אחרת";
const char* KEYWORD_SWITCH = "מחלוקת";
const char* KEYWORD_CASE = "כאשר";
const char* KEYWORD_DEFAULT = "חדל";

const char* OPERATOR_EQUALS = "שווה";
const char* OPERATOR_AGAINST = "לנגד";
const char* OPERATOR_NOT = "שקרי־הוא";
const char* OPERATOR_GREATER = "גדול";
const char* OPERATOR_GREATER_EQUAL = "גדול־ושווה";
const char* OPERATOR_LESS = "קטן";
const char* OPERATOR_LESS_EQUAL = "קטן־ושווה";
const char* OPERATOR_AND = "וגם";
const char* OPERATOR_OR = "או";

const char* KEYWORD_FOR = "וימנה";
const char* KEYWORD_DO = "עשה";
const char* KEYWORD_WHILE = "בעוד";
const char* KEYWORD_CONTINUE = "פסח";
const char* KEYWORD_BREAK ="לך־לך";

const char* COMMENT_MULTILINE = "והגה ה\\' לאמור:(?:.|\\s)*ויחדל.";
const char* COMMENT_ONE_LINER = "והגה ה\\'.*$";

const char* KEYWORD_TRY = "וינסה";
const char* KEYWORD_EXCEPTION = "פסיקה";
const char* KEYWORD_CATCH = "ויתפוס";
const char* KEYWORD_THROW = "וישלך";
const char* KEYWORD_THROWS = "וישלכהו";

const char* KEYWORD_IMPORT = "ויבא";


#endif // CODESH_COMPILER_REGEX_HELPER_H
