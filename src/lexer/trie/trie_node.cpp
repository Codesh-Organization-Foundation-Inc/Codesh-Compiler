#include "trie_node.h"

codesh::lexer::trie::trie_node::trie_node() : keyword(nullptr)
{
}

std::optional<std::reference_wrapper<const codesh::lexer::trie::keyword_info>> codesh::lexer::trie::trie_node::
    get_keyword() const
{
    if (this->keyword == nullptr)
        return std::nullopt;

    return std::cref(*this->keyword);
}

void codesh::lexer::trie::trie_node::set_keyword(const keyword_info *const keyword)
{
    this->keyword = keyword;
}

codesh::lexer::trie::trie_node &codesh::lexer::trie::trie_node::get_or_create_child(const char c)
{
    if (this->children.contains(c))
        return *this->children.at(c);

    return *children.emplace(c, std::make_unique<trie_node>()).first->second;
}

std::optional<std::reference_wrapper<const codesh::lexer::trie::trie_node>> codesh::lexer::trie::trie_node::get_child(
    const char c) const
{
    if (this->children.contains(c))
        return *this->children.at(c);

    return std::nullopt;
}
