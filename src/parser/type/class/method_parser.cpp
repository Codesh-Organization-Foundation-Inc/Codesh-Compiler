#include "method_parser.h"

#include "../../../blasphemies/blasphemy_collector.h"
#include "../../../blasphemies/blasphemy_details.h"
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

        default: error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::UNEXPECTED_TOKEN,
            error::blasphemy_type::SYNTAX);
        }
    }

    error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_SCOPE_END,
        error::blasphemy_type::SYNTAX);
}