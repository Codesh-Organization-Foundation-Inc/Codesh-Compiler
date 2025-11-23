#include "util.h"

#include "ast/type/custom_type_ast_node.h"
#include "ast/type/primitive_type_ast_node.h"

std::unique_ptr<codesh::token> codesh::parser::util::consume_token(std::queue<std::unique_ptr<token>> &tokens)
{
    // TODO: Request custom error message
    ensure_tokens_exist(tokens);

    std::unique_ptr<token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}

std::unique_ptr<codesh::identifier_token> codesh::parser::util::consume_identifier_token(
    std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<token> token = consume_token(tokens);

    if (token->get_group() != token_group::IDENTIFIER)
    {
        throw std::runtime_error("Unexpected token: Expected identifier");
    }

    return std::unique_ptr<identifier_token>(
        static_cast<identifier_token *>(token.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::parser::util::parse_type(
    std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::type::type_ast_node> result;

    const auto type_token = consume_token(tokens);

    switch (type_token->get_group())
    {
    case token_group::KEYWORD_INTEGER:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::INTEGER);
        break;
    case token_group::KEYWORD_FLOAT:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::FLOAT);
        break;
    case token_group::KEYWORD_DOUBLE:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::DOUBLE);
        break;
    case token_group::KEYWORD_LONG:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::LONG);
        break;
    case token_group::KEYWORD_SHORT:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::SHORT);
        break;
    case token_group::KEYWORD_BYTE:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::BYTE);
        break;
    case token_group::KEYWORD_CHAR:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::CHAR);
        break;
    case token_group::KEYWORD_BOOLEAN:
        result = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::BOOLEAN);
        break;

    case token_group::IDENTIFIER:
    {
        const std::string name = static_cast<identifier_token *>(type_token.get())->get_content(); // NOLINT(*-pro-type-static-cast-downcast)
        result = std::make_unique<ast::type::custom_type_ast_node>(name);
        break;
    }

    default:
        throw std::runtime_error("Unexpected token: Invalid type name");
    }


    // Handle arrays
    while (tokens.front()->get_group() == token_group::KEYWORD_ARRAY)
    {
        tokens.pop();
        result->set_array_dimensions(result->get_array_dimensions() + 1);
    }

    return result;
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


void codesh::parser::util::parse_fqcn(std::queue<std::unique_ptr<token>> &tokens, std::list<std::string> &fqcn,
        const bool allow_wildcard)
{
    while (!tokens.empty())
    {
        std::unique_ptr<token> id = consume_token(tokens);

        if (id->get_group() != token_group::IDENTIFIER)
        {
            if (allow_wildcard && id->get_group() == token_group::PUNCTUATION_WILDCARD)
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
