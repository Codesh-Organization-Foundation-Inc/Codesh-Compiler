#include "tokenizer.h"

#include <unicode/uchar.h>
#include "../token/token.h"
#include "regex.h"
#include "trie/trie.h"

namespace trie = codesh::lexer::trie;


static bool is_word_char(const char16_t c) {
    return u_isalnum(c) || c == u'־';
}

static bool check_boundary(const std::u16string &code, const trie::keyword_info *keyword, const size_t start,
                           const size_t end) {
    if (!keyword)
        return false;

    // If there are no boundaries at all, the checks don't really matter.
    if (keyword->boundary == trie::word_boundary::NONE)
        return true;


    if (keyword->boundary == trie::word_boundary::BEFORE || keyword->boundary == trie::word_boundary::BOTH)
    {
        // Check whether a character exists before this keyword
        if (start > 0 && is_word_char(code[start-1]))
            return false;
    }

    if (keyword->boundary == trie::word_boundary::AFTER || keyword->boundary == trie::word_boundary::BOTH)
    {
        // Check whether a character exists after this keyword
        if (end < code.size() && is_word_char(code[end]))
            return false;
    }


    return true;
}

std::queue<std::unique_ptr<codesh::token>> codesh::lexer::tokenize_code(const std::u16string &code)
{
    std::queue<std::unique_ptr<token>> tokens;

    size_t i = 0;
    while (i < code.size())
    {
        if (isspace(code[i]))
        {
            i++;
            continue;
        }

        // Check if comment:
        // if (code.compare(i, 10, "והגה ה' לאמר:") == 0) {
        //     size_t end = code.find("ויחדל:", i);
        //     if (end != std::string::npos) {
        //         tokens.push(token::from_block(code.substr(i, end - i + 7)));
        //         i = end + 7;
        //         continue;
        //     }
        // }

        const trie::trie_node *current = trie::LANGUAGE_TRIE.get();
        const trie::keyword_info *last_match = nullptr;
        size_t last_match_end = i;

        for (size_t j = i; j < code.size() && current->get_child(code[j]); j++)
        {
            current = &current->get_child(code[j])->get();

            if (const auto keyword = current->get_keyword()) {
                last_match = &keyword->get();
                last_match_end = j + 1;
            }
        }

        if (last_match && check_boundary(code, last_match, i, last_match_end))
        {
            tokens.push(std::make_unique<token>(token_type::KEYWORD, last_match->token));
            i = last_match_end;
            continue;
        }


        // If not a keyword, resort to a REGEX literal/identifier check.
        const auto match = *boost::utf16regex_iterator(code.c_str() + i, code.c_str() + code.length(), LEXER_RGX);

        for (int j = 1; j < TOKEN_GROUP_RGX_COUNT; ++j)
        {
            if (const auto &match_info = match[j]; match_info.matched)
            {
                tokens.push(token::from_regex_group_id(j, match_info));
                i += match_info.length();
                break;
            }
        }
    }

    return tokens;
}
