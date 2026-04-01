#pragma once

#include "keyword_info.h"

#include <memory>
#include <optional>
#include <unordered_map>

namespace codesh::lexer::trie
{
struct trie_match
{
    token_group keyword_token;
    word_boundary boundary;
};

class trie_node
{
    std::unordered_map<char16_t, std::unique_ptr<trie_node>> children;
    std::optional<trie_match> match;

public:
    [[nodiscard]] std::optional<trie_match> get_match() const;
    void set_match(const trie_match &match);

    [[nodiscard]] trie_node &get_or_create_child(char16_t c);
    [[nodiscard]] std::optional<std::reference_wrapper<const trie_node>> get_child(char16_t c) const;
};

}
