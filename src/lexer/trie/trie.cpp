#include "trie.h"

#include "keywords.h"


static std::unique_ptr<const codesh::lexer::trie::trie_node> create_language_trie()
{
    auto *const root = new codesh::lexer::trie::trie_node();

    for (const auto &keyword : codesh::lexer::trie::KEYWORDS)
    {
        codesh::lexer::trie::trie_node *current = root;

        for (const auto keyword_char : keyword.keyword)
        {
            current = &current->get_or_create_child(keyword_char);
        }

        current->set_keyword(&keyword);
    }

    return std::unique_ptr<const codesh::lexer::trie::trie_node>(root);
}

const std::unique_ptr<const codesh::lexer::trie::trie_node> codesh::lexer::trie::LANGUAGE_TRIE =
    std::move(create_language_trie());
