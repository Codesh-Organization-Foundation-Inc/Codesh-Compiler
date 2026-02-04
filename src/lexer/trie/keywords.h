#pragma once

#include "keyword_info.h"

#include <unordered_map>
#include <vector>

namespace codesh::lexer::trie
{

extern const std::vector<keyword_info> KEYWORDS;
extern const std::unordered_map<token_group, std::string> TOKEN_TO_NAME_MAP;

namespace keyword
{
constexpr std::u16string MULTILINE_COMMENT_END = u"ויחדל:";

// This is only used in blasphemy messages
constexpr std::string TYPE_VOID = "תֹּהוּ";

// These are not UTF-16 as this is used for post-processing the returned string.
// The returned string is in UTF-8.
constexpr std::string_view STRING_OPEN = "ויקרא ";
constexpr std::string_view STRING_END = " לאמר";

constexpr std::string_view STRING_NEWLINE = " ש\"ח ";

constexpr std::string_view STRING_ESCAPE = "ליטרלי ";


static constexpr std::string ALIAS_STRING = "כתובים";
}

}
