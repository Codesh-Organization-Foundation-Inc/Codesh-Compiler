#include "method_parser.h"

#include "../../util.h"

void codesh::parser::parse_method(std::queue<std::unique_ptr<token>> &tokens)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (util::consume_token(tokens)->get_group())
        {
        case token_group::SCOPE_END:
            return;

        default: throw std::runtime_error("Unexpected token");
        }
    }
}