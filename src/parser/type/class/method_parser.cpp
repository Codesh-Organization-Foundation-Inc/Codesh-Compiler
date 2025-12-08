#include "method_parser.h"

#include "../../../blasphemies/blasphemy_collector.h"
#include "../../util.h"

void codesh::parser::parse_method(std::queue<std::unique_ptr<token>> &tokens)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (tokens.front()->get_group())
        {
        case token_group::SCOPE_END:
            tokens.pop();
            return;

        default: throw std::runtime_error("Unexpected token");
        }
    }

    error::get_blasphemy_collector().add_blasphemy("נָבוֹא שְׁקָרַי: צִפָּה לְ־וַיַתָם", error::blasphemy_type::SYNTAX);
}