#include "util.h"

std::unique_ptr<codesh::token> codesh::parser::util::consume_token(std::queue<std::unique_ptr<token>> &tokens)
{
    // TODO: Request custom error message
    ensure_tokens_exist(tokens);

    std::unique_ptr<token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}

bool codesh::parser::util::consuming_check(std::queue<std::unique_ptr<token>> &tokens, const token_group token_group)
{
    if (!tokens.empty() && tokens.front()->get_group() != token_group)
        return false;

    tokens.pop();
    return true;
}

void codesh::parser::util::ensure_tokens_exist(const std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty())
    {
        throw std::runtime_error("Unexpected EOF"); //TODO: Convert to custom Codesh error
    }
}

void codesh::parser::util::ensure_end_op(std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty() || tokens.front()->get_group() != token_group::PUNCTUATION_END_OP)
    {
        throw std::runtime_error("Unexpected token: Expected colon"); //TODO: Convert to custom Codesh error
    }

    tokens.pop();
}


void codesh::parser::util::parse_fqcn(std::queue<std::unique_ptr<token>> &tokens, std::list<std::string> &fqcn)
{
    while (!tokens.empty())
    {
        std::unique_ptr<token> id = consume_token(tokens);

        if (id->get_group() != token_group::IDENTIFIER)
        {
            if (id->get_group() == token_group::PUNCTUATION_WILDCARD)
            {
                fqcn.emplace_back("*");
            }
            else
            {
                throw std::runtime_error("Unexpected token: Expected identifier"); //TODO: Convert to custom Codesh error
            }
        }
        else
        {
            fqcn.push_back(static_cast<identifier_token *>(id.get())->get_content()); // NOLINT(*-pro-type-static-cast-downcast)
        }


        if (!tokens.empty() && tokens.front()->get_group() == token_group::PUNCTUATION_DOT)
        {
            // Consume the dot
            tokens.pop();
            continue;
        }

        break;
    }

}
