#include "util.h"

#include "../blasphemies/blasphemy_collector.h"
#include "../defenition/fully_qualified_class_name.h"
#include "ast/type/custom_type_ast_node.h"
#include "ast/type/primitive_type_ast_node.h"
#include "../blasphemies/blasphemy_details.h"

std::unique_ptr<codesh::token> codesh::parser::util::consume_token(std::queue<std::unique_ptr<token>> &tokens,
        const std::string &no_tokens_blasphemy_details)
{
    // TODO: Request custom error message
    ensure_tokens_exist(tokens, no_tokens_blasphemy_details);

    std::unique_ptr<token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}

std::unique_ptr<codesh::identifier_token> codesh::parser::util::consume_identifier_token(
        std::queue<std::unique_ptr<token>> &tokens, const std::string &no_tokens_blasphemy_details)
{
    std::unique_ptr<token> token = consume_token(tokens, no_tokens_blasphemy_details);

    if (token->get_group() != token_group::IDENTIFIER)
    {
        error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_IDENTIFIER,
            error::blasphemy_type::SYNTAX, std::nullopt);
        return nullptr; // TODO: check if this return nullptr is okay
    }

    return std::unique_ptr<identifier_token>(
        static_cast<identifier_token *>(token.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::parser::util::parse_type(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::type::type_ast_node> result;

    const auto type_token = consume_token(tokens, error::blasphemy_details::NO_TYPE);

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
        error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_TYPE,
            error::blasphemy_type::SYNTAX, std::nullopt);
        return nullptr;
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
    if (peeking_check(tokens, token_group))
    {
        tokens.pop();
        return true;
    }

    return false;
}

bool codesh::parser::util::peeking_check(const std::queue<std::unique_ptr<token>> &tokens,
        const token_group token_group)
{
    return !tokens.empty() && tokens.front()->get_group() == token_group;
}

void codesh::parser::util::ensure_tokens_exist(const std::queue<std::unique_ptr<token>> &tokens,
        const std::string &no_tokens_blasphemy_details)
{
    if (tokens.empty())
    {
        // TODO: Switch error message to take from parameter
        error::get_blasphemy_collector().add_blasphemy(no_tokens_blasphemy_details,
            error::blasphemy_type::SYNTAX, std::nullopt, true);
    }
}

void codesh::parser::util::ensure_end_op(std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty() || tokens.front()->get_group() != token_group::PUNCTUATION_END_OP)
    {
        error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_PUNCTUATION_END_OP,
            error::blasphemy_type::SYNTAX);
        return;
    }

    tokens.pop();
}


void codesh::parser::util::parse_fqcn(std::queue<std::unique_ptr<token>> &tokens, definition::fully_qualified_class_name &fqcn_out)
{
    while (!tokens.empty())
    {
        std::unique_ptr<token> id = consume_token(tokens, error::blasphemy_details::NO_IDENTIFIER);

        if (id->get_group() != token_group::IDENTIFIER)
        {
            if (id->get_group() == token_group::PUNCTUATION_WILDCARD)
            {
                fqcn_out.set_is_wildcard(true);
            }
            else
            {
                error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_IDENTIFIER,
                    error::blasphemy_type::SYNTAX);
            }
        }
        else
        {
            fqcn_out.add(static_cast<identifier_token *>(id.get())->get_content()); // NOLINT(*-pro-type-static-cast-downcast)
        }


        if (!tokens.empty())
        {
            if (tokens.front()->get_group() == token_group::PUNCTUATION_DOT)
            {
                // Consume the dot
                tokens.pop();
                continue;
            }

            const bool is_last_item = tokens.front()->get_group() == token_group::PUNCTUATION_END_OP;

            // If the user has put a wildcard yet still attempts to add more shit
            if (!is_last_item && fqcn_out.is_wildcard())
            {
                // throw std::runtime_error("Unexpected token: A wildcard statement must be the last item in an FQCN");
                error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_IDENTIFIER,
                    error::blasphemy_type::SYNTAX);
            }
        }

        break;
    }
}
