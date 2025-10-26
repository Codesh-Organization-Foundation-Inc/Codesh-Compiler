#include "tokenizer.h"

#include <unicode/uchar.h>
#include "../token/token.h"
#include "regex.h"
#include "trie/trie.h"


static bool is_word_char(const char32_t c) {
    const auto uc = static_cast<UChar32>(c);
    return u_isalnum(uc) || c == U'־';
}

static bool check_boundary(const codesh::lexer::trie::keyword_info* kw, const std::u32string& code, size_t start, size_t end) {
    if (!kw) return false;
    if ((kw->boundary == codesh::lexer::trie::word_boundary::BEFORE || kw->boundary == codesh::lexer::trie::word_boundary::BOTH) &&
        start > 0 && is_word_char(code[start-1])) return false;
    if ((kw->boundary == codesh::lexer::trie::word_boundary::AFTER || kw->boundary == codesh::lexer::trie::word_boundary::BOTH) &&
        end < code.size() && is_word_char(code[end])) return false;
    return true;
}

std::queue<std::unique_ptr<codesh::token>> codesh::lexer::tokenize_code(const std::u32string code)
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
        size_t j = i;
        const trie::keyword_info *last_match = nullptr;
        size_t last_match_end = i;

        while (j < code.size() && current->get_child(code[j])) {
            current = &current->get_child(code[j])->get();
            j++;
            if (const auto keyword = current->get_keyword()) {
                last_match = &keyword->get();
                last_match_end = j;
            }
        }

        if (last_match && check_boundary(last_match, code, i, last_match_end)) {
            tokens.push(std::make_unique<token>(token_type::KEYWORD, last_match->token));
            i = last_match_end;
            continue;
        }



    }

    return tokens;
}
