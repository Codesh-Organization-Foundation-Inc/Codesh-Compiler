#pragma once

#include "token/token_group.h"
#include "word_boundary.h"

#include <string>

namespace codesh::lexer::trie
{

struct keyword_info
{
    std::u16string keyword;
    word_boundary boundary;
};

}
