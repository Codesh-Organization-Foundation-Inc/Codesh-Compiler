#pragma once

#include "keyword_info.h"

#include <memory>
#include <optional>
#include <unordered_map>

namespace codesh::lexer::trie
{

class trie_node
{
    std::unordered_map<char, std::unique_ptr<trie_node>> children;
    const std::unique_ptr<const keyword_info> keyword;

public:
    trie_node();
    explicit trie_node(std::unique_ptr<keyword_info> keyword);

    [[nodiscard]] std::optional<std::reference_wrapper<const keyword_info>> get_keyword() const;
    [[nodiscard]] std::unordered_map<char, std::unique_ptr<trie_node>> &get_children();
};

}
