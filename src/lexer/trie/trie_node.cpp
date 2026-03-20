#include "trie_node.h"

std::optional<codesh::lexer::trie::trie_match> codesh::lexer::trie::trie_node::get_match() const
{
    return this->match;
}

void codesh::lexer::trie::trie_node::set_match(const trie_match &match)
{
    this->match.emplace(match);
}

codesh::lexer::trie::trie_node &codesh::lexer::trie::trie_node::get_or_create_child(const char16_t c)
{
    if (this->children.contains(c))
        return *this->children.at(c);

    return *children.emplace(c, std::make_unique<trie_node>()).first->second;
}

std::optional<std::reference_wrapper<const codesh::lexer::trie::trie_node>> codesh::lexer::trie::trie_node::get_child(
    const char16_t c) const
{
    if (this->children.contains(c))
        return *this->children.at(c);

    return std::nullopt;
}
