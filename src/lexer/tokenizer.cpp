#include "tokenizer.h"

#include "../token/token.h"
#include "regex.h"
#include "trie/trie.h"

std::queue<std::unique_ptr<codesh::token>> codesh::lexer::tokenize_code(const std::string &code)
{
    // boost::u32regex_iterator it(code.begin(), code.end(), LEXER_RGX);
    // const boost::u32regex_iterator<std::string::const_iterator> end;
    //
    // std::queue<std::unique_ptr<token>> tokens;
    //
    // while (it != end) {
    //     auto match = *it;
    //
    //     for (int i = 1; i < static_cast<int>(token_group::COUNT) + 1; ++i)
    //     {
    //         if (match[i].matched)
    //         {
    //             tokens.push(token::from_group_id(i, match[i]));
    //             break;
    //         }
    //     }
    //
    //     ++it;
    // }

    std::queue<std::unique_ptr<token>> tokens;

    // size_t i = 0;
    // while (i < code.size())
    // {
    //     if (isspace(code[i]))
    //     {
    //         i++;
    //         continue;
    //     }
    //
    //
    //     const trie::trie_node *current = trie::LANGUAGE_TRIE.get();
    // }

    return tokens;
}
