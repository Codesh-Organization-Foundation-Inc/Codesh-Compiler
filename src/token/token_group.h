#pragma once

namespace codesh
{

enum class token_group : int
{
    KEYWORD_BASAD, // בס"ד
    KEYWORD_BH, // בע"ה
    KEYWORD_IAW, // אי"א

    SCOPE_BEGIN, // ויאמר:
    SCOPE_END, // ויתם:

    OPEN_PARENTHESIS, // כי־
    CLOSE_PARENTHESIS, // כי־טוב
    TOV, // טוב

    KEYWORD_ORIGIN_COUNTRY, // וישב בארץ
    KEYWORD_IMPORT, // ויבא
    KEYWORD_IMPORT_REGULAR, // את
    KEYWORD_IMPORT_STATIC, // לכל־את

    KEYWORD_LET, // ויהי
    KEYWORD_NAME, // ושמו/ושמה
    KEYWORD_SHALL_BE, // היה/תהיה
    KEYWORD_RETURN, // וישב

    KEYWORD_METHOD, // מעשה
    KEYWORD_CONSTRUCTOR, // בריאה
    KEYWORD_FUNCTION_CALL, // ויעש
    KEYWORD_TAKES, // ויקח

    KEYWORD_CLASS, // עצם
    KEYWORD_ENUM, // מניין
    KEYWORD_INTERFACE, // צלם

    KEYWORD_PUBLIC, // נגלה
    KEYWORD_PRIVATE, // נחבא
    KEYWORD_PROTECTED, // קדש
    KEYWORD_STATIC, // לכל־
    KEYWORD_FINAL, // ימות ולא־יתחלף

    KEYWORD_ANNOTATION, // חותם
    KEYWORD_ANNOTATE, // והיה
    KEYWORD_ANNOTATE_OVERRIDE, // ויגזול

    KEYWORD_EXTENDS, //בן־
    KEYWORD_IMPLEMENTS, // בצלם
    KEYWORD_NEW, // ויברא
    KEYWORD_INSTANCE, // מזרע
    KEYWORD_ABSTRACT, // רוחני
    KEYWORD_THIS, // אנכי
    KEYWORD_SUPER, // אבי
    KEYWORD_AS, // בתור־

    OPERATOR_ADDITION, // ויוסף ל-
    OPERATOR_SUBTRACTION, // ויוחסר מן
    OPERATOR_MULTIPLICATION, // ויוכפל
    OPERATOR_DIVISION, // ויחולק
    OPERATOR_MODULO, // ויושרה
    OPERATOR_MINUS, // ויחזק את־לב
    OPERATOR_BY, // פי־
    OPERATOR_THE, // את־

    OPERATOR_ADDITION_ASSIGNMENT, // ויסף ל־
    OPERATOR_SUBTRACTION_ASSIGNMENT, // ויחסר מן
    OPERATOR_MULTIPLICATION_ASSIGNMENT, // ויכפל
    OPERATOR_DIVISION_ASSIGNMENT, // ויחלק
    OPERATOR_MODULO_ASSIGNMENT, // וישרה

    OPERATOR_INCREMENT, // ויתקדם
    OPERATOR_DECREMENT, // ויפחת

    KEYWORD_VAR, // משתנה
    KEYWORD_INTEGER, // שלם
    KEYWORD_FLOAT, // צף
    KEYWORD_DOUBLE, // מצף
    KEYWORD_LONG, // רב
    KEYWORD_SHORT, // קצר
    KEYWORD_BYTE, // קצרצר
    KEYWORD_CHAR, // אות
    KEYWORD_BOOLEAN, // דבר־מה
    KEYWORD_NULL, // תהו

    KEYWORD_ARRAY, // כמסדר
    KEYWORD_INDEX, // במקום
    KEYWORD_REPLACE, // ויחלף
    PUNCTUATION_EQUAL, // ב־

    KEYWORD_IF, // אם־
    KEYWORD_ELSE, // אחרת
    KEYWORD_ELSE_IF, // אם־אחרת
    KEYWORD_SWITCH, // ויחלוק על
    KEYWORD_CASE, // כאשר
    KEYWORD_DEFAULT, // באין צדיק

    KEYWORD_TRUE, // אמן
    KEYWORD_FALSE, // כזב

    OPERATOR_NOT, // שקרי־הוא
    OPERATOR_EQUALS, // שווה
    OPERATOR_NOT_EQUALS, // אינו־שווה
    OPERATOR_GREATER, // גדול
    OPERATOR_GREATER_EQUALS, // גדול־ושווה
    OPERATOR_LESS, // קטן
    OPERATOR_LESS_EQUALS, // קטן־ושווה
    OPERATOR_AGAINST, // לנגד
    OPERATOR_AND, // וגם
    OPERATOR_OR, // או

    PUNCTUATION_END_OP, // :
    PUNCTUATION_ARG_SEPARATOR, // ו־
    PUNCTUATION_DOT, // ל־
    PUNCTUATION_WILDCARD, // כולל

    KEYWORD_FOR, // וימנה
    KEYWORD_FROM, // מן
    KEYWORD_TO, // עד
    KEYWORD_SKIP, // וידלג
    KEYWORD_DO, // עשה
    KEYWORD_WHILE, // בעוד
    KEYWORD_CONTINUE, // פסח
    KEYWORD_BREAK, // לך־לך

    KEYWORD_TRY, // וינסה
    KEYWORD_CATCH, // ויתפוס
    KEYWORD_FINALLY, // לאחר ארבעים שנה במדבר
    KEYWORD_THROW, // גזר
    KEYWORD_THROWS, // פן־יחטא

    COMMENT_MULTILINE, // כה הגה ה' לאמור: ... ויחדל:
    COMMENT_ONE_LINER, // כה הגה ה'


    // Biblical numbers
    KEYWORD_BIBLICAL_DECIMAL_SEPARATOR, // נקודה

    KEYWORD_BIBLICAL_ZERO, // חמס
    KEYWORD_BIBLICAL_ONE, // אחד
    KEYWORD_BIBLICAL_TWO, // שתיים
    KEYWORD_BIBLICAL_THREE, // שלוש
    KEYWORD_BIBLICAL_FOUR, // ארבע
    KEYWORD_BIBLICAL_FIVE, // חמש
    KEYWORD_BIBLICAL_SIX, // שש
    KEYWORD_BIBLICAL_SEVEN, // שבע
    KEYWORD_BIBLICAL_EIGHT, // שמונה
    KEYWORD_BIBLICAL_NINE, // תשע
    KEYWORD_BIBLICAL_TEN, // עשר
    KEYWORD_BIBLICAL_TWELVE, // תריסר
    KEYWORD_BIBLICAL_TWENTY, // עשרים
    KEYWORD_BIBLICAL_THIRTY, // שלושים
    KEYWORD_BIBLICAL_FORTY, // ארבעים
    KEYWORD_BIBLICAL_FIFTY, // חמישים
    KEYWORD_BIBLICAL_SIXTY, // שישים
    KEYWORD_BIBLICAL_SEVENTY, // שבעים
    KEYWORD_BIBLICAL_EIGHTY, // שמונים
    KEYWORD_BIBLICAL_NINETY, // תשעים
    KEYWORD_BIBLICAL_HUNDRED, // מאה
    KEYWORD_BIBLICAL_THOUSAND, // אלף
    KEYWORD_BIBLICAL_TEN_THOUSAND, // רבבה

    // Biblical numbers prefixed by vav (plus)
    KEYWORD_BIBLICAL_ZERO_ADDED,  // וחמס
    KEYWORD_BIBLICAL_ONE_ADDED,  // ואחד
    KEYWORD_BIBLICAL_TWO_ADDED,  // ושתיים
    KEYWORD_BIBLICAL_THREE_ADDED,  // ושלוש
    KEYWORD_BIBLICAL_FOUR_ADDED,  // וארבע
    KEYWORD_BIBLICAL_FIVE_ADDED,  // וחמש
    KEYWORD_BIBLICAL_SIX_ADDED,  // ושש
    KEYWORD_BIBLICAL_SEVEN_ADDED,  // ושבע
    KEYWORD_BIBLICAL_EIGHT_ADDED,  // ושמונה
    KEYWORD_BIBLICAL_NINE_ADDED,  // ותשע
    KEYWORD_BIBLICAL_TEN_ADDED,  // ועשר
    KEYWORD_BIBLICAL_TWELVE_ADDED,  // ותריסר
    KEYWORD_BIBLICAL_TWENTY_ADDED,  // ועשרים
    KEYWORD_BIBLICAL_THIRTY_ADDED,  // ושלושים
    KEYWORD_BIBLICAL_FORTY_ADDED,  // וארבעים
    KEYWORD_BIBLICAL_FIFTY_ADDED,  // וחמישים
    KEYWORD_BIBLICAL_SIXTY_ADDED,  // ושישים
    KEYWORD_BIBLICAL_SEVENTY_ADDED,  // ושבעים
    KEYWORD_BIBLICAL_EIGHTY_ADDED,  // ושמונים
    KEYWORD_BIBLICAL_NINETY_ADDED,  // ותשעים
    KEYWORD_BIBLICAL_HUNDRED_ADDED,  // ומאה
    KEYWORD_BIBLICAL_THOUSAND_ADDED,  // ואלף
    KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED,  // ורבבה


    // REGEX Matches

    //TODO: Match with definition::primitive_type
    LITERAL_NUMBER_FLOAT, // x.y צף | x צף
    LITERAL_NUMBER_DOUBLE, // x.y
    LITERAL_NUMBER_INT, // x
    LITERAL_STRING, // "פסוק"
    LITERAL_CHAR, // 'אות'

    IDENTIFIER, // Any other word, eg. variable1

    INVALID
};

}
