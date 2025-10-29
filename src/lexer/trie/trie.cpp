#include "trie.h"

#include "keywords.h"

using namespace codesh::lexer::trie;

static std::unique_ptr<const trie_node> create_language_trie()
{
    trie_node *const root = new trie_node();

    for (const keyword_info &keyword : KEYWORDS)
    {
        trie_node *current = root;

        for (const char16_t keyword_char : keyword.keyword)
        {
            current = &current->get_or_create_child(keyword_char);
        }

        current->set_keyword(&keyword);
    }

    return std::unique_ptr<const trie_node>(root);
}

const std::unique_ptr<const trie_node> codesh::lexer::trie::LANGUAGE_TRIE = std::move(create_language_trie());
