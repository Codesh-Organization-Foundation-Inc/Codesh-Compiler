#include "tokenizer.h"

#include "regex.h"
#include "../token/token.h"

std::queue<std::unique_ptr<codesh::token>> codesh::tokenize_code(const std::string &code)
{
    boost::u32regex_iterator it(code.begin(), code.end(), LEXER_RGX);
    const boost::u32regex_iterator<std::string::const_iterator> end;

    std::queue<std::unique_ptr<token>> tokens;

    while (it != end) {
        auto match = *it;

        for (int i = 1; i < static_cast<int>(token_group::COUNT) + 1; ++i)
        {
            if (match[i].matched)
            {
                tokens.push(token::from_group_id(i, match[i]));
                break;
            }
        }

        ++it;
    }

    return tokens;
}
