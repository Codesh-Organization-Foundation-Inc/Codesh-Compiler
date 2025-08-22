#pragma once

#include <string_view>

namespace codesh::token
{

enum token_type
{
    SCOPE_BEGIN, // ויאמר:
    SCOPE_END, // ויתם.

    KEYWORD_LET, // ויהי
    KEYWORD_RETURN, // וישב

    IDENTIFIER_NUMBER, // any number (with digit 0 - 9)
    IDENTIFIER_CUSTOM, // any other word, eg. variable1

    OPERATOR_ADDITION, // ויוסף ל-
    OPERATOR_SUBTRACTION, // ויוחסר מן
    OPERATOR_MULTIPLICATION, // ויוכפל
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

    SEMICOLON, //;
    DOT, // ל־

    KEYWORD_CLASS, // עצם
    KEYWORD_ENUM, // מניין
    KEYWORD_THIS, // אנוכי
    KEYWORD_FUNCTION, // המעשה
    KEYWORD_FUNCTION_CALL, // ויעש

    KEYWORD_PUBLIC, // נגלה|נגלית
    KEYWORD_PRIVATE, // נסתר|נסתרת
    KEYWORD_STATIC, // כללי
    KEYWORD_ABSTRACT, // רוחני
    KEYWORD_FINAL, // וימות ולא־יתחלף
    KEYWORD_SUPER, // אבי

    KEYWORD_VAR, //משתנה
    KEYWORD_INTEGER, // שלם
    KEYWORD_FLOAT, // צף
    KEYWORD_DOUBLE, //מצף
    KEYWORD_LONG, // רב
    KEYWORD_SHORT, // קצר
    KEYWORD_BYTE, // קצרצר
    KEYWORD_CHAR, // תו
    KEYWORD_STRING, // פסוק
    KEYWORD_BOOLEAN, // דבר־מה
    KEYWORD_NULL, //תהו

    KEYWORD_IF, // אם
    KEYWORD_ELSE, // אחרת
    KEYWORD_SWITCH, // מחלוקת
    KEYWORD_CASE, // כאשר
    KEYWORD_DEFAULT, // חדל

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
    KEYWORD_DO, // עשה
    KEYWORD_WHILE, // בעוד
    KEYWORD_CONTINUE, // פסח
    KEYWORD_BREAK, // לך־לך

    COMMENT_MULTILINE, // והגה ה' לאמור: ... ויחדל.
    COMMENT_ONE_LINER, // והגה ה'

    KEYWORD_TRY, // וינסה
    KEYWORD_EXCEPTION, // פסיקה
    KEYWORD_CATCH, // ויתפוס
    KEYWORD_THROW, // וישלך
    KEYWORD_THROWS, // וישלכהו

    KEYWORD_IMPORT // ויבא
};

token_type get_token_type(std::string_view group_name);

}
