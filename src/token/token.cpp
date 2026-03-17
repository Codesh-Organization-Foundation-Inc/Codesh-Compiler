#include "token.h"

#include "lexer/regex.h"

#include <utf8/checked.h>
#include <utility>

using codesh::token_group;

codesh::token_type codesh::token::get_token_type(const token_group name)
{
    switch (name)
    {
    case token_group::IDENTIFIER:
    case token_group::LITERAL_NUMBER_INT:
    case token_group::LITERAL_NUMBER_FLOAT:
    case token_group::LITERAL_NUMBER_DOUBLE:
    case token_group::LITERAL_STRING:
    case token_group::LITERAL_CHAR:
        return token_type::IDENTIFIER;

    default: return token_type::KEYWORD;
    }
}

codesh::token::token(const lexer::code_position code_position, const token_type type, const token_group group) :
    code_position(code_position),
    type(type),
    group(group)
{}

codesh::token::~token() = default;


std::unique_ptr<codesh::token> codesh::token::from_regex_group_id(lexer::code_position code_position,
        const int group_id, const std::u16string &content)
{
    const token_group group = lexer::token_group_from_regex_id(group_id);

    switch (const token_type type = get_token_type(group))
    {
    case token_type::IDENTIFIER:
        // Convert the contents back to UTF-8 as we don't really need it except for lexing.
        return std::make_unique<identifier_token>(code_position, group, utf8::utf16to8(content));

    default:
        return std::make_unique<token>(code_position, type, group);
    }
}

codesh::lexer::code_position codesh::token::get_code_position() const
{
    return code_position;
}

codesh::token_type codesh::token::get_type() const
{
    return this->type;
}

token_group codesh::token::get_group() const
{
    return this->group;
}

codesh::identifier_token::identifier_token(const lexer::code_position code_position, const token_group group,
        std::string content) :
    token(code_position, token_type::IDENTIFIER, group),
    content(std::move(content))
{
}

std::string codesh::identifier_token::get_content() const
{
    return this->content;
}

void codesh::identifier_token::set_content(std::string content)
{
    this->content = std::move(content);
}
