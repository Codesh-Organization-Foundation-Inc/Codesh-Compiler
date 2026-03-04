#include "keywords.h"

#include <utf8.h>

const std::vector<codesh::lexer::trie::keyword_info> codesh::lexer::trie::KEYWORDS = {
    {token_group::KEYWORD_BASAD, u"בס\"ד", word_boundary::BOTH},
    {token_group::KEYWORD_BH, u"בע\"ה", word_boundary::BOTH},
    {token_group::KEYWORD_IAW, u"אי\"א", word_boundary::BOTH},

    {token_group::SCOPE_BEGIN, u"ויאמר:", word_boundary::BEFORE},
    {token_group::SCOPE_END, u"ויתם:", word_boundary::BEFORE},

    {token_group::OPEN_PARENTHESIS, u"כי־", word_boundary::BEFORE},
    {token_group::CLOSE_PARENTHESIS, u"כי־טוב", word_boundary::BOTH},
    {token_group::TOV, u"טוב", word_boundary::BOTH},

    {token_group::KEYWORD_ORIGIN_COUNTRY, u"וישב בארץ", word_boundary::BOTH},
    {token_group::KEYWORD_IMPORT, u"ויבא", word_boundary::BOTH},
    {token_group::KEYWORD_IMPORT_STATIC, u"לכל־את", word_boundary::BOTH},
    {token_group::KEYWORD_IMPORT_REGULAR, u"את", word_boundary::BOTH},

    {token_group::KEYWORD_LET, u"ויהי", word_boundary::BOTH},
    {token_group::KEYWORD_NAME, u"ושמו", word_boundary::BOTH},
    {token_group::KEYWORD_NAME, u"ושמה", word_boundary::BOTH},
    {token_group::KEYWORD_SHALL_BE, u"היה", word_boundary::BOTH},
    {token_group::KEYWORD_SHALL_BE, u"תהיה", word_boundary::BOTH},
    {token_group::KEYWORD_RETURN, u"וישב", word_boundary::BOTH},

    {token_group::KEYWORD_METHOD, u"מעשה", word_boundary::BOTH},
    {token_group::KEYWORD_CONSTRUCTOR, u"בריאה", word_boundary::BOTH},
    {token_group::KEYWORD_FUNCTION_CALL, u"ויעש", word_boundary::BOTH},
    {token_group::KEYWORD_TAKES, u"ויקח", word_boundary::BOTH},

    {token_group::KEYWORD_CLASS, u"עצם", word_boundary::BOTH},
    {token_group::KEYWORD_ENUM, u"מניין", word_boundary::BOTH},
    {token_group::KEYWORD_INTERFACE, u"צלם", word_boundary::BOTH},

    {token_group::KEYWORD_PUBLIC, u"נגלה", word_boundary::BOTH},
    {token_group::KEYWORD_PRIVATE, u"נחבא", word_boundary::BOTH},
    {token_group::KEYWORD_PROTECTED, u"קדש", word_boundary::BOTH},
    {token_group::KEYWORD_STATIC, u"לכל־", word_boundary::BEFORE},
    {token_group::KEYWORD_FINAL, u"ימות ולא־יתחלף", word_boundary::BOTH},

    {token_group::KEYWORD_ANNOTATION, u"חותם", word_boundary::BOTH},
    {token_group::KEYWORD_ANNOTATE, u"והיה", word_boundary::BOTH},
    {token_group::KEYWORD_ANNOTATE_OVERRIDE, u"ויגזול", word_boundary::BOTH},

    {token_group::KEYWORD_EXTENDS, u"בן־", word_boundary::BEFORE},
    {token_group::KEYWORD_IMPLEMENTS, u"בצלם", word_boundary::BOTH},
    {token_group::KEYWORD_NEW, u"ויברא", word_boundary::BOTH},
    {token_group::KEYWORD_INSTANCE, u"מזרע", word_boundary::BOTH},
    {token_group::KEYWORD_ABSTRACT, u"רוחני", word_boundary::BOTH},
    {token_group::KEYWORD_THIS, u"אנכי", word_boundary::BOTH},
    {token_group::KEYWORD_SUPER, u"אבי", word_boundary::BOTH},

    {token_group::OPERATOR_ADDITION, u"ויוסף ל־", word_boundary::BEFORE},
    {token_group::OPERATOR_SUBTRACTION, u"ויוחסר מן", word_boundary::BOTH},
    {token_group::OPERATOR_MULTIPLICATION, u"ויוכפל", word_boundary::BOTH},
    {token_group::OPERATOR_DIVISION, u"ויחולק", word_boundary::BOTH},
    {token_group::OPERATOR_MODULO, u"ויושרה", word_boundary::BOTH},
    {token_group::OPERATOR_MINUS, u"ויחזק את־לב", word_boundary::BOTH},
    {token_group::OPERATOR_BY, u"פי־", word_boundary::BEFORE},

    {token_group::OPERATOR_ADDITION_ASSIGNMENT, u"ויסף ל־", word_boundary::BEFORE},
    {token_group::OPERATOR_SUBTRACTION_ASSIGNMENT, u"ויחסר מן", word_boundary::BOTH},
    {token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT, u"ויכפל", word_boundary::BOTH},
    {token_group::OPERATOR_DIVISION_ASSIGNMENT, u"ויחלק", word_boundary::BOTH},
    {token_group::OPERATOR_MODULO_ASSIGNMENT, u"וישרה", word_boundary::BOTH},

    {token_group::OPERATOR_INCREMENT, u"ויתקדם", word_boundary::BOTH},
    {token_group::OPERATOR_DECREMENT, u"ויפחת", word_boundary::BOTH},

    {token_group::KEYWORD_VAR, u"משתנה", word_boundary::BOTH},
    {token_group::KEYWORD_INTEGER, u"שלם", word_boundary::BOTH},
    {token_group::KEYWORD_FLOAT, u"צף", word_boundary::BOTH},
    {token_group::KEYWORD_DOUBLE, u"מצף", word_boundary::BOTH},
    {token_group::KEYWORD_LONG, u"רב", word_boundary::BOTH},
    {token_group::KEYWORD_SHORT, u"קצר", word_boundary::BOTH},
    {token_group::KEYWORD_BYTE, u"קצרצר", word_boundary::BOTH},
    {token_group::KEYWORD_CHAR, u"אות", word_boundary::BOTH},
    {token_group::KEYWORD_BOOLEAN, u"דבר־מה", word_boundary::BOTH},
    {token_group::KEYWORD_NULL, u"תהו", word_boundary::BOTH},

    {token_group::KEYWORD_ARRAY, u"כמסדר", word_boundary::BOTH},
    {token_group::KEYWORD_INDEX, u"במקום", word_boundary::BOTH},
    {token_group::KEYWORD_REPLACE, u"ויחלף", word_boundary::BOTH},
    {token_group::PUNCTUATION_EQUAL, u"ב־", word_boundary::BEFORE},

    {token_group::KEYWORD_IF, u"אם־", word_boundary::BEFORE},
    {token_group::KEYWORD_ELSE, u"אחרת", word_boundary::BOTH},
    {token_group::KEYWORD_ELSE_IF, u"אם־אחרת", word_boundary::BOTH},
    {token_group::KEYWORD_SWITCH, u"ויחלוק על", word_boundary::BOTH},
    {token_group::KEYWORD_CASE, u"כאשר", word_boundary::BOTH},
    {token_group::KEYWORD_DEFAULT, u"באין צדיק", word_boundary::BOTH},

    {token_group::KEYWORD_TRUE, u"אמן", word_boundary::BOTH},
    {token_group::KEYWORD_FALSE, u"כזב", word_boundary::BOTH},

    {token_group::OPERATOR_NOT, u"שקרי־הוא", word_boundary::BOTH},
    {token_group::OPERATOR_EQUALS, u"שווה", word_boundary::BOTH},
    {token_group::OPERATOR_NOT_EQUALS, u"אינו־שווה", word_boundary::BOTH},
    {token_group::OPERATOR_GREATER, u"גדול", word_boundary::BOTH},
    {token_group::OPERATOR_GREATER_EQUALS, u"גדול־ושווה", word_boundary::BOTH},
    {token_group::OPERATOR_LESS, u"קטן", word_boundary::BOTH},
    {token_group::OPERATOR_LESS_EQUALS, u"קטן־ושווה", word_boundary::BOTH},
    {token_group::OPERATOR_AGAINST, u"לנגד", word_boundary::BOTH},
    {token_group::OPERATOR_AND, u"וגם", word_boundary::BOTH},
    {token_group::OPERATOR_OR, u"או", word_boundary::BOTH},

    {token_group::PUNCTUATION_END_OP, u":", word_boundary::NONE},
    {token_group::PUNCTUATION_ARG_SEPARATOR, u"ו־", word_boundary::BEFORE},
    {token_group::PUNCTUATION_DOT, u"ל־", word_boundary::BEFORE},
    {token_group::PUNCTUATION_WILDCARD, u"כולל", word_boundary::BOTH},

    {token_group::KEYWORD_FOR, u"וימנה", word_boundary::BOTH},
    {token_group::KEYWORD_FROM, u"מן", word_boundary::BOTH},
    {token_group::KEYWORD_TO, u"עד", word_boundary::BOTH},
    {token_group::KEYWORD_SKIP, u"וידלג", word_boundary::BOTH},
    {token_group::KEYWORD_DO, u"עשה", word_boundary::BOTH},
    {token_group::KEYWORD_WHILE, u"בעוד", word_boundary::BOTH},
    {token_group::KEYWORD_CONTINUE, u"פסח", word_boundary::BOTH},
    {token_group::KEYWORD_BREAK, u"לך־לך", word_boundary::BOTH},

    {token_group::KEYWORD_TRY, u"וינסה", word_boundary::BOTH},
    {token_group::KEYWORD_CATCH, u"ויתפוס", word_boundary::BOTH},
    {token_group::KEYWORD_FINALLY, u"לאחר ארבעים שנה במדבר", word_boundary::BOTH},
    {token_group::KEYWORD_THROW, u"גזר", word_boundary::BOTH},
    {token_group::KEYWORD_THROWS, u"פן־יחטא", word_boundary::BOTH},

    {token_group::KEYWORD_BIBLICAL_DECIMAL_SEPARATOR, u"נקודה", word_boundary::BOTH},

    {token_group::KEYWORD_BIBLICAL_ZERO, u"חמס", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_ONE, u"אחד", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TWO, u"שתיים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_THREE, u"שלוש", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FOUR, u"ארבע", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FIVE, u"חמש", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SIX, u"שש", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SEVEN, u"שבע", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_EIGHT, u"שמונה", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_NINE, u"תשע", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TEN, u"עשר", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TWELVE, u"תריסר", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TWENTY, u"עשרים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_THIRTY, u"שלושים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FORTY, u"ארבעים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FIFTY, u"חמישים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SIXTY, u"שישים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SEVENTY, u"שבעים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_EIGHTY, u"שמונים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_NINETY, u"תשעים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_HUNDRED, u"מאה", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_THOUSAND, u"אלף", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TEN_THOUSAND, u"רבבה", word_boundary::BOTH},

    {token_group::KEYWORD_BIBLICAL_ZERO_ADDED, u"וחמס", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_ONE_ADDED, u"ואחד", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TWO_ADDED, u"ושתיים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_THREE_ADDED, u"ושלוש", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FOUR_ADDED, u"וארבע", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FIVE_ADDED, u"וחמש", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SIX_ADDED, u"ושש", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SEVEN_ADDED, u"ושבע", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_EIGHT_ADDED, u"ושמונה", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_NINE_ADDED, u"ותשע", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TEN_ADDED, u"ועשר", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TWELVE_ADDED, u"ותריסר", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TWENTY_ADDED, u"ועשרים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_THIRTY_ADDED, u"ושלושים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FORTY_ADDED, u"וארבעים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_FIFTY_ADDED, u"וחמישים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SIXTY_ADDED, u"ושישים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_SEVENTY_ADDED, u"ושבעים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_EIGHTY_ADDED, u"ושמונים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_NINETY_ADDED, u"ותשעים", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_HUNDRED_ADDED, u"ומאה", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_THOUSAND_ADDED, u"ואלף", word_boundary::BOTH},
    {token_group::KEYWORD_BIBLICAL_TEN_THOUSAND_ADDED, u"ורבבה", word_boundary::BOTH},

    {token_group::COMMENT_MULTILINE, u"ויאמר ה' לאמר:", word_boundary::BEFORE},
    {token_group::COMMENT_MULTILINE, u"וכה הגה ה' לאמר:", word_boundary::BEFORE},
    {token_group::COMMENT_MULTILINE, u"ויאמר ה' לאמר:", word_boundary::BEFORE},
    {token_group::COMMENT_ONE_LINER, u"וכה הגה ה' לאמר", word_boundary::BOTH},
};

static std::unordered_map<codesh::token_group, std::string> token_to_name_map()
{
    std::unordered_map<codesh::token_group, std::string> result;

    for (const auto &entry : codesh::lexer::trie::KEYWORDS)
    {
        // The token name map is used primarily in blasphemy details,
        // and UTF-16 isn't really a necessity for them.
        result.emplace(entry.token, utf8::utf16to8(entry.keyword));
    }

    return result;
}

const std::unordered_map<codesh::token_group, std::string> codesh::lexer::trie::TOKEN_TO_NAME_MAP = token_to_name_map();
