#pragma once

namespace codesh
{

enum class token_group : int
{
    SCOPE_BEGIN, // ויאמר:
    SCOPE_END, // ויתם:

    KEYWORD_LET, // ויהי
    KEYWORD_NAME, // ושמו
    KEYWORD_SHALL_BE, // יהיה
    KEYWORD_RETURN, // וישב

    //
    IDENTIFIER_NUMBER_DOUBLE, // x.y
    IDENTIFIER_NUMBER_FLOAT, // x.y צף | x צף
    IDENTIFIER_NUMBER_INT, // x
    IDENTIFIER_STRING, // "פסוק"
    IDENTIFIER_CHAR, // 'תו'
    IDENTIFIER_CUSTOM, // Any other word, eg. variable1

    OPERATOR_ADDITION, // ויוסף ל-
    OPERATOR_SUBTRACTION, // ויוחסר מן
    OPERATOR_MULTIPLICATION, // ויוכפל
    OPERATOR_TIMES, // פי
    OPERATOR_DIVISION, // ויחולק
    OPERATOR_MODULO, // ויושרה

    OPERATOR_ADDITION_ASSIGNMENT, //ויסף
    OPERATOR_SUBTRACTION_ASSIGNMENT, // ויחסר
    OPERATOR_MULTIPLICATION_ASSIGNMENT, // ויכפל
    OPERATOR_DIVISION_ASSIGNMENT, // ויחלק
    OPERATOR_MODULO_ASSIGNMENT, // וישרה

    OPERATOR_INCREMENT, // ויתקדם
    OPERATOR_DECREMENT, // ויפחת

    OPEN_PARENTHESIS, // כי
    CLOSE_PARENTHESIS, // פסק

    PUNCTUATION_END_OP, // :
    PUNCTUATION_COMMA, // ,
    PUNCTUATION_DOT, // ל־

    KEYWORD_CLASS, // עצם
    KEYWORD_ENUM, // מניין
    KEYWORD_THIS, // אנכי
    KEYWORD_FUNCTION, // המעשה
    KEYWORD_FUNCTION_CALL, // ויעש

    KEYWORD_PUBLIC, // נגלה|נגלית
    KEYWORD_PRIVATE, // נסתר|נסתרת
    KEYWORD_STATIC, // כללי
    KEYWORD_ABSTRACT, // רוחני
    KEYWORD_FINAL, // ימות ולא־יתחלף
    KEYWORD_SUPER, // אבי

    KEYWORD_VAR, // משתנה
    KEYWORD_INTEGER, // שלם
    KEYWORD_FLOAT, // צף
    KEYWORD_DOUBLE, // מצף
    KEYWORD_LONG, // רב
    KEYWORD_SHORT, // קצר
    KEYWORD_BYTE, // קצרצר
    KEYWORD_CHAR, // תו
    KEYWORD_STRING, // פסוק
    KEYWORD_BOOLEAN, // דבר־מה
    KEYWORD_NULL, // תהו
    KEYWORD_ARRAY, // כמערך

    KEYWORD_REPLACE, // ויחלף

    KEYWORD_IF, // אם
    KEYWORD_ELSE, // אחרת
    KEYWORD_SWITCH, // מחלוקת
    KEYWORD_CASE, // כאשר
    KEYWORD_DEFAULT, // חדל

    KEYWORD_TRUE, // אמן
    KEYWORD_FALSE, // כזב

    OPERATOR_EQUALS, // שווה ... לנגד
    OPERATOR_NOT, // שקרי־הוא
    OPERATOR_GREATER, // גדול
    OPERATOR_GREATER_EQUAL, // גדול־ושווה
    OPERATOR_LESS, // קטן
    OPERATOR_LESS_EQUAL, // קטן־ושווה
    OPERATOR_AND, // וגם
    OPERATOR_OR, // או

    OPERATOR_AGAINST, // לנגד


    KEYWORD_FOR, // וימנה
    KEYWORD_FROM, // מן
    KEYWORD_TO, // עד
    KEYWORD_SKIP, // וידלג
    KEYWORD_DO, // עשה
    KEYWORD_WHILE, // בעוד
    KEYWORD_CONTINUE, // פסח
    KEYWORD_BREAK, // לך־לך


    COMMENT_MULTILINE, // והגה ה' לאמור: ... ויחדל:
    COMMENT_ONE_LINER, // והגה ה'

    KEYWORD_TRY, // וינסה
    KEYWORD_EXCEPTION, // פסיקה
    KEYWORD_CATCH, // ויתפוס
    KEYWORD_THROW, // וישלך
    KEYWORD_THROWS, // וישלכהו

    KEYWORD_IMPORT, // ויבא

    KEYWORD_PACKAGE, // וישב בארץ
    KEYWORD_INDEX, // במקום

    COUNT // Dummy entry that equals the amount of entries in the enum
};

}
