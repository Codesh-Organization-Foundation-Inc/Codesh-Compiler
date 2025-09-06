#pragma once

#include "keyword_info.h"

#include <vector>

namespace codesh::lexer::trie
{

extern const std::vector<keyword_info> KEYWORDS;

namespace keyword
{
extern constexpr std::string_view MULTILINE_COMMENT_END;
}

}
