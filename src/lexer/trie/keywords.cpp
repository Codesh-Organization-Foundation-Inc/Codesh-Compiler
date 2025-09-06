#include "keywords.h"

constexpr std::string_view codesh::lexer::trie::keyword::MULTILINE_COMMENT_END = "ויחדל:";

const std::vector<codesh::lexer::trie::keyword_info> codesh::lexer::trie::KEYWORDS = {
    {token_group::COMMENT_MULTILINE, "ויאמר ה' לאמר:", word_boundary::BEFORE},
    {token_group::COMMENT_MULTILINE, "וכה הגה ה' לאמר:", word_boundary::BEFORE},
    {token_group::COMMENT_ONE_LINER, "ויאמר ה' לאמר", word_boundary::BOTH},

    {token_group::PUNCTUATION_ARG_SEPARATOR, "ו־", word_boundary::BEFORE},
    {token_group::PUNCTUATION_DOT, "ל־", word_boundary::BEFORE},
    {token_group::PUNCTUATION_WILDCARD, "כולל", word_boundary::BOTH},

    {token_group::KEYWORD_PACKAGE, "וישב בארץ", word_boundary::BOTH},
    {token_group::KEYWORD_IMPORT_STATIC, "ויבא בכלל", word_boundary::BOTH},
    {token_group::KEYWORD_IMPORT, "ויבא", word_boundary::BOTH},

    {token_group::SCOPE_BEGIN, "ויאמר:", word_boundary::BEFORE},
    {token_group::SCOPE_END, "ויתם:", word_boundary::BEFORE},

    {token_group::KEYWORD_LET, "ויהי", word_boundary::BOTH},
    {token_group::KEYWORD_NAME, "ושמו", word_boundary::BOTH},
    {token_group::KEYWORD_SHALL_BE, "(?:יהיה|תהיה)", word_boundary::BOTH},
    {token_group::KEYWORD_RETURN, "וישב", word_boundary::BOTH},

    {token_group::OPERATOR_ADDITION, "ויוסף ל־", word_boundary::BEFORE},
    {token_group::OPERATOR_SUBTRACTION, "ויוחסר מן", word_boundary::BOTH},
    {token_group::OPERATOR_MULTIPLICATION, "ויוכפל", word_boundary::BOTH},
    {token_group::OPERATOR_TIMES, "פי", word_boundary::BOTH},
    {token_group::OPERATOR_DIVISION, "ויחולק", word_boundary::BOTH},
    {token_group::OPERATOR_MODULO, "ויושרה", word_boundary::BOTH},

    {token_group::OPERATOR_ADDITION_ASSIGNMENT, "ויסף", word_boundary::BOTH},
    {token_group::OPERATOR_SUBTRACTION_ASSIGNMENT, "ויחסר", word_boundary::BOTH},
    {token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT, "ויכפל", word_boundary::BOTH},
    {token_group::OPERATOR_DIVISION_ASSIGNMENT, "ויחלק", word_boundary::BOTH},
    {token_group::OPERATOR_MODULO_ASSIGNMENT, "וישרה", word_boundary::BOTH},

    {token_group::OPERATOR_INCREMENT, "ויתקדם", word_boundary::BOTH},
    {token_group::OPERATOR_DECREMENT, "ויפחת", word_boundary::BOTH},

    {token_group::OPEN_PARENTHESIS, "כי", word_boundary::BOTH},
    {token_group::CLOSE_PARENTHESIS, "פסק", word_boundary::BOTH},

    {token_group::KEYWORD_CLASS, "עצם", word_boundary::BOTH},
    {token_group::KEYWORD_ENUM, "מניין", word_boundary::BOTH},
    {token_group::KEYWORD_INTERFACE, "צלם", word_boundary::BOTH},
    {token_group::KEYWORD_THIS, "אנכי", word_boundary::BOTH},
    {token_group::KEYWORD_IMPLEMENTS, "וירש את", word_boundary::BOTH},
    {token_group::KEYWORD_OF_INSTANCE, "מזרע", word_boundary::BOTH},
    {token_group::KEYWORD_NEW, "ויברא", word_boundary::BOTH},

    {token_group::KEYWORD_FUNCTION, "המעשה", word_boundary::BOTH},
    {token_group::KEYWORD_FUNCTION_CALL, "ויעש", word_boundary::BOTH},
    {token_group::KEYWORD_FUNCTION_TAKE_ARGS, "ויקח", word_boundary::BOTH},

    {token_group::KEYWORD_PUBLIC, "נגלה", word_boundary::BOTH},
    {token_group::KEYWORD_PUBLIC, "נגלית", word_boundary::BOTH},
    {token_group::KEYWORD_PRIVATE, "נסתר", word_boundary::BOTH},
    {token_group::KEYWORD_PRIVATE, "נסתרת", word_boundary::BOTH},
    {token_group::KEYWORD_STATIC, "כללי", word_boundary::BOTH},
    {token_group::KEYWORD_ABSTRACT, "רוחני", word_boundary::BOTH},
    {token_group::KEYWORD_FINAL, "ימות ולא־יתחלף", word_boundary::BOTH},
    {token_group::KEYWORD_SUPER, "אבי", word_boundary::BOTH},

    {token_group::KEYWORD_VAR, "משתנה", word_boundary::BOTH},
    {token_group::KEYWORD_INTEGER, "שלם", word_boundary::BOTH},
    {token_group::KEYWORD_FLOAT, "צף", word_boundary::BOTH},
    {token_group::KEYWORD_DOUBLE, "מצף", word_boundary::BOTH},
    {token_group::KEYWORD_LONG, "רב", word_boundary::BOTH},
    {token_group::KEYWORD_SHORT, "קצר", word_boundary::BOTH},
    {token_group::KEYWORD_BYTE, "קצרצר", word_boundary::BOTH},
    {token_group::KEYWORD_CHAR, "אות", word_boundary::BOTH},
    {token_group::KEYWORD_STRING, "פסוק", word_boundary::BOTH},
    {token_group::KEYWORD_BOOLEAN, "דבר־מה", word_boundary::BOTH},
    {token_group::KEYWORD_NULL, "תהו", word_boundary::BOTH},
    {token_group::KEYWORD_ARRAY, "כמסדר", word_boundary::BOTH},
    {token_group::KEYWORD_INDEX, "במקום", word_boundary::BOTH},

    {token_group::KEYWORD_REPLACE, "ויחלף", word_boundary::BOTH},

    {token_group::KEYWORD_IF, "אם", word_boundary::BOTH},
    {token_group::KEYWORD_ELSE, "אחרת", word_boundary::BOTH},
    {token_group::KEYWORD_SWITCH, "ויחלוק על", word_boundary::BOTH},
    {token_group::KEYWORD_CASE, "אם־כי", word_boundary::BOTH},
    {token_group::KEYWORD_DEFAULT, "באין צדיק", word_boundary::BOTH},

    {token_group::KEYWORD_TRUE, "אמן", word_boundary::BOTH},
    {token_group::KEYWORD_FALSE, "כזב", word_boundary::BOTH},

    {token_group::OPERATOR_EQUALS, "שווה", word_boundary::BOTH},
    {token_group::OPERATOR_NOT_EQUALS, "אינו־שווה", word_boundary::BOTH},
    {token_group::OPERATOR_AGAINST, "לנגד", word_boundary::BOTH},
    {token_group::OPERATOR_NOT, "שקרי־הוא", word_boundary::BOTH},
    {token_group::OPERATOR_GREATER, "גדול", word_boundary::BOTH},
    {token_group::OPERATOR_GREATER_EQUAL, "גדול־ושווה", word_boundary::BOTH},
    {token_group::OPERATOR_LESS, "קטן", word_boundary::BOTH},
    {token_group::OPERATOR_LESS_EQUAL, "קטן־ושווה", word_boundary::BOTH},
    {token_group::OPERATOR_AND, "וגם", word_boundary::BOTH},
    {token_group::OPERATOR_OR, "או", word_boundary::BOTH},


    {token_group::KEYWORD_FOR, "וימנה", word_boundary::BOTH},
    {token_group::KEYWORD_FROM, "מן", word_boundary::BOTH},
    {token_group::KEYWORD_TO, "עד", word_boundary::BOTH},
    {token_group::KEYWORD_SKIP, "וידלג", word_boundary::BOTH},

    {token_group::KEYWORD_DO, "עשה", word_boundary::BOTH},
    {token_group::KEYWORD_WHILE, "בעוד", word_boundary::BOTH},
    {token_group::KEYWORD_CONTINUE, "פסח", word_boundary::BOTH},
    {token_group::KEYWORD_BREAK, "לך־לך", word_boundary::BOTH},


    {token_group::KEYWORD_TRY, "וינסה", word_boundary::BOTH},
    {token_group::KEYWORD_EXCEPTION, "פסיקה", word_boundary::BOTH},
    {token_group::KEYWORD_CATCH, "ויתפוס", word_boundary::BOTH},
    {token_group::KEYWORD_THROW, "וישלך", word_boundary::BOTH},
    {token_group::KEYWORD_THROWS, "וישלכהו", word_boundary::BOTH},

    {token_group::PUNCTUATION_END_OP, ":", word_boundary::NONE},
};