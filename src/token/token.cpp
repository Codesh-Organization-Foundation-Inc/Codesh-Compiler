#include "token.h"

#include "../lexer/regex.h"

#include <utility>

using codesh::token_group;

codesh::token_type codesh::token::get_token_type(const token_group name)
{
    switch (name)
    {
    case token_group::IDENTIFIER_CUSTOM:
    case token_group::IDENTIFIER_NUMBER_INT:
    case token_group::IDENTIFIER_NUMBER_FLOAT:
    case token_group::IDENTIFIER_NUMBER_DOUBLE:
    case token_group::IDENTIFIER_STRING:
    case token_group::IDENTIFIER_CHAR:
        return token_type::IDENTIFIER;

    default: return token_type::KEYWORD;
    }
}

codesh::token::token(const token_type type, const token_group group) :
    type(type),
    group(group)
{}

codesh::token::~token() = default;


codesh::token *codesh::token::from_group_id(const int group_id, const std::string &content)
{
    const token_group group = token_group_from_regex_id(group_id);

    switch (const token_type type = get_token_type(group))
    {
    case token_type::IDENTIFIER:
        return new identifier_token(type, group, content);
    default:
        return new token(type, group);
    }
}

codesh::token_type codesh::token::get_type() const
{
    return this->type;
}

token_group codesh::token::get_group() const
{
    return this->group;
}

codesh::identifier_token::identifier_token(const token_type type, const token_group group, std::string content) :
    token(type, group),
    content(std::move(content))
{}
