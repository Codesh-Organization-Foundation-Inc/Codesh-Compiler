#include "trie.h"

#include "keywords.h"

static std::unique_ptr<const codesh::lexer::trie::trie_node> create_language_trie()
{
    auto root = std::make_unique<codesh::lexer::trie::trie_node>();

    for (const auto &[keyword_token, keyword_info] : codesh::lexer::trie::KEYWORDS)
    {
        auto *current = root.get();

        for (const auto keyword_char : keyword_info.keyword)
        {
            current = &current->get_or_create_child(keyword_char);
        }

        current->set_match({keyword_token, keyword_info.boundary});
    }

    return std::move(root);
}

const std::unique_ptr<const codesh::lexer::trie::trie_node> codesh::lexer::trie::LANGUAGE_TRIE = create_language_trie();
