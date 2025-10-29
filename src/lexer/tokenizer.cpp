#include "tokenizer.h"

#include "../token/token.h"
#include "regex.h"
#include "trie/keywords.h"
#include "trie/trie.h"
#include <unicode/uchar.h>

namespace trie = codesh::lexer::trie;

/**
 * @return How many characters this character should consume.
 */
static size_t get_keyword_consume_size(const std::u16string &code, const codesh::token *token, size_t keyword_end);


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
            i = get_keyword_consume_size(code, tokens.back().get(), last_match_end);
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

static size_t get_keyword_consume_size(const std::u16string &code, const codesh::token *const token,
                                       const size_t keyword_end)
{
    switch (token->get_group())
    {
        case codesh::token_group::COMMENT_ONE_LINER: {
            // If it's a one-line comment, look for the end of the line.
            const size_t end = code.find(u'\n', keyword_end);

            if (end != std::string::npos)
                return end;

            return keyword_end;
        }

        case codesh::token_group::COMMENT_MULTILINE: {
            // If it's a multiline comment, look for a trie::keyword::MULTILINE_COMMENT_END match.
            const size_t end = code.find(trie::keyword::MULTILINE_COMMENT_END, keyword_end);

            if (end != std::string::npos)
                return end + trie::keyword::MULTILINE_COMMENT_END.length();

            //TODO: Convert to error token or alike
            throw std::runtime_error("Unenclosed multiline comment");
        }

        default: return keyword_end;
    }
}
