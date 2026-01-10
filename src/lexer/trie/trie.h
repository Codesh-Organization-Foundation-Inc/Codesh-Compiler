#pragma once

#include "lexer/trie/trie_node.h"

#include <memory>

namespace codesh::lexer::trie
{

extern const std::unique_ptr<const trie_node> LANGUAGE_TRIE;

}
