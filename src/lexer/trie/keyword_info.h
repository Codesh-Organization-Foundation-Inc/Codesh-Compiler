#pragma once

#include "../../token/token_group.h"
#include "word_boundary.h"

namespace codesh::lexer::trie
{

struct keyword_info
{
    token_group token;
    word_boundary boundary;
};

}
