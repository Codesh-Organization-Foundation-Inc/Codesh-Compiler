#pragma once

#include "keyword_info.h"

#include <memory>
#include <optional>
#include <unordered_map>

namespace codesh::lexer::trie
{

class trie_node
{
    std::unordered_map<char16_t, std::unique_ptr<trie_node>> children;
    const keyword_info *keyword;

public:
    trie_node();

    [[nodiscard]] std::optional<std::reference_wrapper<const keyword_info>> get_keyword() const;
    void set_keyword(const keyword_info *keyword);

    [[nodiscard]] trie_node &get_or_create_child(char16_t c);
    [[nodiscard]] std::optional<std::reference_wrapper<const trie_node>> get_child(char16_t c) const;
};

}
