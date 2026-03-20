#pragma once

#include "keyword_info.h"

#include <unordered_map>
#include <vector>

namespace codesh::lexer::trie
{
using keywords_map = std::unordered_map<token_group, keyword_info>;
extern const keywords_map KEYWORDS;
extern const keywords_map KEYWORDS_FEMININE;

std::string token_to_string(token_group token, bool prefer_feminine = false);

namespace keyword
{
inline constexpr std::u16string MULTILINE_COMMENT_END = u"ויחדל:";

// This is only used in blasphemy messages
inline constexpr std::string TYPE_VOID = "תֹּהוּ";

// These are not UTF-16 as this is used for post-processing the returned string.
// The returned string is in UTF-8.
inline constexpr std::string_view STRING_OPEN = "ויקרא ";
inline constexpr std::string_view STRING_END = " לאמר";

inline constexpr std::string_view STRING_NEWLINE = " ש\"ח ";

inline constexpr std::string_view STRING_ESCAPE = "ליטרלי ";
}
}