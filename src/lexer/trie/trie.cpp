#include "trie.h"

#include "keywords.h"

static void add_keywords_to_trie(codesh::lexer::trie::trie_node &root,
        const codesh::lexer::trie::keywords_map &keywords);


static std::unique_ptr<const codesh::lexer::trie::trie_node> create_language_trie()
{
    auto root = std::make_unique<codesh::lexer::trie::trie_node>();

    add_keywords_to_trie(*root, codesh::lexer::trie::KEYWORDS);
    add_keywords_to_trie(*root, codesh::lexer::trie::KEYWORDS_FEMININE);

    return std::move(root);
}

static void add_keywords_to_trie(codesh::lexer::trie::trie_node &root,
        const codesh::lexer::trie::keywords_map &keywords)
{
    for (const auto &[keyword_token, keyword_info] : keywords)
    {
        auto *current = &root;

        for (const auto keyword_char : keyword_info.keyword)
        {
            current = &current->get_or_create_child(keyword_char);
        }

        current->set_match({keyword_token, keyword_info.boundary});
    }
}

const std::unique_ptr<const codesh::lexer::trie::trie_node> codesh::lexer::trie::LANGUAGE_TRIE = create_language_trie();
