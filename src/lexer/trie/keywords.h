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
constexpr std::string_view MULTILINE_COMMENT_END = "כי־טוב:";

constexpr std::string_view STRING_OPEN = "ויקרא ";
constexpr std::string_view STRING_END = " לאמר";

constexpr std::string_view STRING_NEWLINE = " ש\"ח ";

constexpr std::string_view STRING_ESCAPE = "ליטרלי ";
}

}
