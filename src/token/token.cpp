#include "token.h"

#include "../lexer/group_name.h"

#include <unordered_map>
#include <utility>

using codesh::token_group;
using namespace codesh::regex::group_name;


static const std::unordered_map<std::string_view, token_group> GROUP_NAME_TO_TOKEN_TYPE = {
    {IDENTIFIER_NUMBER_GROUP_NAME, token_group::IDENTIFIER_NUMBER},
    {IDENTIFIER_CUSTOM_GROUP_NAME, token_group::IDENTIFIER_CUSTOM},

    {KEYWORD_IMPORT_GROUP_NAME, token_group::KEYWORD_IMPORT},

    {SCOPE_BEGIN_GROUP_NAME, token_group::SCOPE_BEGIN},
    {SCOPE_END_GROUP_NAME, token_group::SCOPE_END},

    {KEYWORD_LET_GROUP_NAME, token_group::KEYWORD_LET},
    {KEYWORD_NAME_GROUP_NAME, token_group::KEYWORD_NAME},
    {KEYWORD_SHALL_BE_GROUP_NAME, token_group::KEYWORD_SHALL_BE},
    {KEYWORD_RETURN_GROUP_NAME, token_group::KEYWORD_RETURN},

    {OPERATOR_ADDITION_GROUP_NAME, token_group::OPERATOR_ADDITION},
    {OPERATOR_SUBTRACTION_GROUP_NAME, token_group::OPERATOR_SUBTRACTION},
    {OPERATOR_MULTIPLICATION_GROUP_NAME, token_group::OPERATOR_MULTIPLICATION},
    {OPERATOR_DIVISION_GROUP_NAME, token_group::OPERATOR_DIVISION},
    {OPERATOR_MODULO_GROUP_NAME, token_group::OPERATOR_MODULO},

    {OPERATOR_ADDITION_ASSIGNMENT_GROUP_NAME, token_group::OPERATOR_ADDITION_ASSIGNMENT},
    {OPERATOR_SUBTRACTION_ASSIGNMENT_GROUP_NAME, token_group::OPERATOR_SUBTRACTION_ASSIGNMENT},
    {OPERATOR_MULTIPLICATION_ASSIGNMENT_GROUP_NAME, token_group::OPERATOR_MULTIPLICATION_ASSIGNMENT},
    {OPERATOR_DIVISION_ASSIGNMENT_GROUP_NAME, token_group::OPERATOR_DIVISION_ASSIGNMENT},
    {OPERATOR_MODULO_ASSIGNMENT_GROUP_NAME, token_group::OPERATOR_MODULO_ASSIGNMENT},

    {OPERATOR_INCREMENT_GROUP_NAME, token_group::OPERATOR_INCREMENT},
    {OPERATOR_DECREMENT_GROUP_NAME, token_group::OPERATOR_DECREMENT},

    {OPEN_PARENTHESIS_GROUP_NAME, token_group::OPEN_PARENTHESIS},
    {CLOSE_PARENTHESIS_GROUP_NAME, token_group::CLOSE_PARENTHESIS},

    {PUNCTUATION_SEMICOLON_GROUP_NAME, token_group::PUNCTUATION_SEMICOLON},
    {PUNCTUATION_COMMA_GROUP_NAME, token_group::PUNCTUATION_COMMA},
    {PUNCTUATION_DOT_GROUP_NAME, token_group::PUNCTUATION_DOT},

    {KEYWORD_CLASS_GROUP_NAME, token_group::KEYWORD_CLASS},
    {KEYWORD_ENUM_GROUP_NAME, token_group::KEYWORD_ENUM},
    {KEYWORD_THIS_GROUP_NAME, token_group::KEYWORD_THIS},
    {KEYWORD_FUNCTION_GROUP_NAME, token_group::KEYWORD_FUNCTION},
    {KEYWORD_FUNCTION_CALL_GROUP_NAME, token_group::KEYWORD_FUNCTION_CALL},

    {KEYWORD_PUBLIC_GROUP_NAME, token_group::KEYWORD_PUBLIC},
    {KEYWORD_PRIVATE_GROUP_NAME, token_group::KEYWORD_PRIVATE},
    {KEYWORD_STATIC_GROUP_NAME, token_group::KEYWORD_STATIC},
    {KEYWORD_ABSTRACT_GROUP_NAME, token_group::KEYWORD_ABSTRACT},
    {KEYWORD_FINAL_GROUP_NAME, token_group::KEYWORD_FINAL},
    {KEYWORD_SUPER_GROUP_NAME, token_group::KEYWORD_SUPER},

    {KEYWORD_VAR_GROUP_NAME, token_group::KEYWORD_VAR},
    {KEYWORD_INTEGER_GROUP_NAME, token_group::KEYWORD_INTEGER},
    {KEYWORD_FLOAT_GROUP_NAME, token_group::KEYWORD_FLOAT},
    {KEYWORD_DOUBLE_GROUP_NAME, token_group::KEYWORD_DOUBLE},
    {KEYWORD_LONG_GROUP_NAME, token_group::KEYWORD_LONG},
    {KEYWORD_SHORT_GROUP_NAME, token_group::KEYWORD_SHORT},
    {KEYWORD_BYTE_GROUP_NAME, token_group::KEYWORD_BYTE},
    {KEYWORD_CHAR_GROUP_NAME, token_group::KEYWORD_CHAR},
    {KEYWORD_STRING_GROUP_NAME, token_group::KEYWORD_STRING},
    {KEYWORD_BOOLEAN_GROUP_NAME, token_group::KEYWORD_BOOLEAN},
    {KEYWORD_NULL_GROUP_NAME, token_group::KEYWORD_NULL},

    {KEYWORD_IF_GROUP_NAME, token_group::KEYWORD_IF},
    {KEYWORD_ELSE_GROUP_NAME, token_group::KEYWORD_ELSE},
    {KEYWORD_SWITCH_GROUP_NAME, token_group::KEYWORD_SWITCH},
    {KEYWORD_CASE_GROUP_NAME, token_group::KEYWORD_CASE},
    {KEYWORD_DEFAULT_GROUP_NAME, token_group::KEYWORD_DEFAULT},

    {OPERATOR_EQUALS_GROUP_NAME, token_group::OPERATOR_EQUALS},
    {OPERATOR_AGAINST_GROUP_NAME, token_group::OPERATOR_AGAINST},
    {OPERATOR_NOT_GROUP_NAME, token_group::OPERATOR_NOT},
    {OPERATOR_GREATER_GROUP_NAME, token_group::OPERATOR_GREATER},
    {OPERATOR_GREATER_EQUAL_GROUP_NAME, token_group::OPERATOR_GREATER_EQUAL},
    {OPERATOR_LESS_GROUP_NAME, token_group::OPERATOR_LESS},
    {OPERATOR_LESS_EQUAL_GROUP_NAME, token_group::OPERATOR_LESS_EQUAL},
    {OPERATOR_AND_GROUP_NAME, token_group::OPERATOR_AND},
    {OPERATOR_OR_GROUP_NAME, token_group::OPERATOR_OR},

    {KEYWORD_FOR_GROUP_NAME, token_group::KEYWORD_FOR},
    {KEYWORD_DO_GROUP_NAME, token_group::KEYWORD_DO},
    {KEYWORD_WHILE_GROUP_NAME, token_group::KEYWORD_WHILE},
    {KEYWORD_CONTINUE_GROUP_NAME, token_group::KEYWORD_CONTINUE},
    {KEYWORD_BREAK_GROUP_NAME, token_group::KEYWORD_BREAK},

    {COMMENT_MULTILINE_GROUP_NAME, token_group::COMMENT_MULTILINE},
    {COMMENT_ONE_LINER_GROUP_NAME, token_group::COMMENT_ONE_LINER},

    {KEYWORD_TRY_GROUP_NAME, token_group::KEYWORD_TRY},
    {KEYWORD_EXCEPTION_GROUP_NAME, token_group::KEYWORD_EXCEPTION},
    {KEYWORD_CATCH_GROUP_NAME, token_group::KEYWORD_CATCH},
    {KEYWORD_THROW_GROUP_NAME, token_group::KEYWORD_THROW},
    {KEYWORD_THROWS_GROUP_NAME, token_group::KEYWORD_THROWS},
};

token_group codesh::token::get_token_group(const std::string_view name)
{
    return GROUP_NAME_TO_TOKEN_TYPE.at(name);
}


codesh::token_type codesh::token::get_token_type(const token_group name)
{
    switch (name)
    {
    case IDENTIFIER_CUSTOM:
    case IDENTIFIER_NUMBER:
        return IDENTIFIER;

    default: return KEYWORD;
    }
}

codesh::token::token(const token_type type, const token_group group) :
    type(type),
    group(group)
{}

codesh::token::~token() = default;


codesh::token *codesh::token::from_group_name(const std::string &group_name, const std::string &content)
{
    const token_group group = get_token_group(group_name);

    switch (const token_type type = get_token_type(group))
    {
    case IDENTIFIER:
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
