#pragma once

#include "token/token_group.h"
#include "lexer/trie/word_boundary.h"

#include <string>

namespace codesh::lexer::trie
{

struct keyword_info
{
    token_group token;
    std::string keyword;
    word_boundary boundary;
};

}
