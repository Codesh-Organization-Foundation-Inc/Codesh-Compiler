#include "trie_node.h"

codesh::lexer::trie::trie_node::trie_node() = default;

codesh::lexer::trie::trie_node::trie_node(std::unique_ptr<keyword_info> keyword) : keyword(std::move(keyword))
{
}

std::optional<std::reference_wrapper<const codesh::lexer::trie::keyword_info>> codesh::lexer::trie::trie_node::
    get_keyword() const
{
    if (this->keyword == nullptr)
        return std::nullopt;

    return std::cref(*this->keyword);
}

std::unordered_map<char, std::unique_ptr<codesh::lexer::trie::trie_node>> &codesh::lexer::trie::trie_node::
    get_children()
{
    return this->children;
}
