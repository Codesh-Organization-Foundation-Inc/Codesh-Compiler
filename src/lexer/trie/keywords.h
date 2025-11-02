#pragma once

#include "keyword_info.h"

#include <vector>

namespace codesh::lexer::trie
{

extern const std::vector<keyword_info> KEYWORDS;

namespace keyword
{
constexpr std::u16string_view MULTILINE_COMMENT_END = u"כי־טוב:";

// This is not UTF-16 as this is used for post-processing the returned string.
// The returned string is in UTF-8.
constexpr std::string_view STRING_OPEN = "יקרא ";
constexpr std::string_view STRING_END = " לאמר";

constexpr std::string_view STRING_NEWLINE = " ש\"ח ";

constexpr std::string_view STRING_ESCAPE = "ליטרלי ";
}

}
