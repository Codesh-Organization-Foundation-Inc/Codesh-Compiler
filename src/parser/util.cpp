#include "util.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "defenition/definitions.h"
#include "defenition/fully_qualified_name.h"
#include "lexer/trie/keywords.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"

#include <cassert>

#ifndef NDEBUG
static auto UNEXPECTED_EOF_ASSERTION = "Unexpected EOF blasphemy that wasn't meant to happen. "
      "Or we just forgot to add an error message.";
#endif


static std::unique_ptr<codesh::identifier_token> make_error_identifier_token(
        codesh::blasphemy::code_position code_position);


std::unique_ptr<codesh::token> codesh::parser::util::consume_token(std::queue<std::unique_ptr<token>> &tokens,
        const std::optional<std::string> &no_tokens_blasphemy_details)
{
    ensure_tokens_exist(tokens, no_tokens_blasphemy_details);

    std::unique_ptr<token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}

std::unique_ptr<codesh::identifier_token> codesh::parser::util::consume_identifier_token(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<token> token = consume_token(tokens, blasphemy::details::NO_IDENTIFIER);

    if (token->get_group() != token_group::IDENTIFIER)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IDENTIFIER,
            blasphemy::blasphemy_type::SYNTAX, token->get_code_position());

        return make_error_identifier_token(token->get_code_position());
    }

    return std::unique_ptr<identifier_token>(
        static_cast<identifier_token *>(token.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );
}

std::unique_ptr<codesh::identifier_token> codesh::parser::util::consume_alnum_identifier_token(
        std::queue<std::unique_ptr<token>> &tokens, const std::optional<std::string> &no_tokens_blasphemy_details)
{
    std::unique_ptr<token> token = consume_token(tokens, no_tokens_blasphemy_details);

    if (token::get_token_type(token->get_group()) != token_type::IDENTIFIER)
    {
        assert(no_tokens_blasphemy_details.has_value() && UNEXPECTED_EOF_ASSERTION);

        blasphemy::get_blasphemy_collector().add_blasphemy(
            *no_tokens_blasphemy_details,
            blasphemy::blasphemy_type::SYNTAX,
            token->get_code_position()
        );

        return make_error_identifier_token(token->get_code_position());
    }

    return std::unique_ptr<identifier_token>(
        static_cast<identifier_token *>(token.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );
}

static std::unique_ptr<codesh::identifier_token> make_error_identifier_token(
        codesh::blasphemy::code_position code_position)
{
    return std::make_unique<codesh::identifier_token>(
        code_position,
        codesh::token_group::IDENTIFIER,
        codesh::definition::ERROR_IDENTIFIER_CONTENT
    );
}


std::unique_ptr<codesh::ast::type::type_ast_node> codesh::parser::util::parse_type(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::type::type_ast_node> result;

    ensure_tokens_exist(tokens, blasphemy::details::NO_TYPE);
    const auto type_pos = tokens.front()->get_code_position();
    const auto token_group = tokens.front()->get_group();

    switch (token_group)
    {
    case token_group::KEYWORD_INTEGER:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::INTEGER);
        break;
    case token_group::KEYWORD_FLOAT:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::FLOAT);
        break;
    case token_group::KEYWORD_DOUBLE:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::DOUBLE);
        break;
    case token_group::KEYWORD_LONG:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::LONG);
        break;
    case token_group::KEYWORD_SHORT:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::SHORT);
        break;
    case token_group::KEYWORD_BYTE:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::BYTE);
        break;
    case token_group::KEYWORD_CHAR:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::CHAR);
        break;
    case token_group::KEYWORD_BOOLEAN:
        result = std::make_unique<ast::type::primitive_type_ast_node>(type_pos, definition::primitive_type::BOOLEAN);
        break;

    case token_group::IDENTIFIER:
    {
        definition::fully_qualified_name name(type_pos);
        parse_fqn(tokens, name);

        result = std::make_unique<ast::type::custom_type_ast_node>(type_pos, name);
        break;
    }

    default:
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_TYPE,
            blasphemy::blasphemy_type::SYNTAX,
            type_pos
        );

        tokens.pop();

        return std::make_unique<ast::type::custom_type_ast_node>(
            type_pos,
            definition::fully_qualified_name(type_pos, definition::ERROR_IDENTIFIER_CONTENT)
        );
    }


    // Only an identifier type does not consume tokens (as the FQN parser does it).
    // Hence, if it isn't, manually consume it:
    if (token_group != token_group::IDENTIFIER)
    {
        tokens.pop();
    }


    // Handle arrays
    while (tokens.front()->get_group() == token_group::KEYWORD_ARRAY)
    {
        tokens.pop();
        result->set_array_dimensions(result->get_array_dimensions() + 1);
    }

    return result;
}

bool codesh::parser::util::consuming_check(std::queue<std::unique_ptr<token>> &tokens, const token_group token_group,
        const std::optional<std::reference_wrapper<std::unique_ptr<token>>> token_out)
{
    if (peeking_check(tokens, token_group))
    {
        if (token_out.has_value())
        {
            token_out.value().get() = std::move(tokens.front());
        }

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
                                               const std::optional<std::string> &no_tokens_blasphemy_details)
{
    if (tokens.empty())
    {
        assert(no_tokens_blasphemy_details.has_value() && UNEXPECTED_EOF_ASSERTION);

        blasphemy::get_blasphemy_collector().add_blasphemy(
            *no_tokens_blasphemy_details,
            blasphemy::blasphemy_type::SYNTAX,
            blasphemy::NO_CODE_POS,
            true
        );
    }
}

void codesh::parser::util::ensure_end_op(std::queue<std::unique_ptr<token>> &tokens)
{
    if (!consuming_check(tokens, token_group::PUNCTUATION_END_OP))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_PUNCTUATION_END_OP,
            blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
        );
    }
}


void codesh::parser::util::parse_fqn(std::queue<std::unique_ptr<token>> &tokens, definition::fully_qualified_name &fqn_out)
{
    while (!tokens.empty())
    {
        std::unique_ptr<token> id = consume_token(tokens, blasphemy::details::NO_IDENTIFIER);

        if (id->get_group() != token_group::IDENTIFIER)
        {
            if (id->get_group() == token_group::PUNCTUATION_WILDCARD)
            {
                fqn_out.set_is_wildcard(true);
            }
            else
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(
                    blasphemy::details::NO_IDENTIFIER,
                    blasphemy::blasphemy_type::SYNTAX,
                    id->get_code_position()
                );
            }
        }
        else
        {
            fqn_out.add(static_cast<identifier_token *>(id.get())->get_content()); // NOLINT(*-pro-type-static-cast-downcast)
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
            if (!is_last_item && fqn_out.is_wildcard())
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(
                    blasphemy::details::NO_IDENTIFIER,
                    blasphemy::blasphemy_type::SYNTAX,
                    tokens.front()->get_code_position()
                );
            }
        }

        break;
    }
}

std::string codesh::parser::util::get_token_display_name(const token &token)
{
    if (const auto id = dynamic_cast<const identifier_token *>(&token))
        return id->get_content();

    const auto it = lexer::trie::TOKEN_TO_NAME_MAP.find(token.get_group());
    if (it != lexer::trie::TOKEN_TO_NAME_MAP.end())
        return it->second;

    return definition::ERROR_IDENTIFIER_CONTENT;
}

bool codesh::parser::util::consume_by(std::queue<std::unique_ptr<token>> &tokens)
{
    if (!consuming_check(tokens, token_group::OPERATOR_BY)) {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_KEYWORD_BY,
            blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
        );

        return false;
    }

    return true;
}

void codesh::parser::util::parse_this_and_fqn(std::queue<std::unique_ptr<token>> &tokens,
        definition::fully_qualified_name &fqn_out)
{
    if (consuming_check(tokens, token_group::KEYWORD_THIS))
    {
        consuming_check(tokens, token_group::PUNCTUATION_DOT);
        fqn_out.add("this");
    }
    parse_fqn(tokens, fqn_out);
}

bool codesh::parser::util::consume_punc_equal(std::queue<std::unique_ptr<token>> &tokens)
{
    if (!consuming_check(tokens, token_group::PUNCTUATION_EQUAL)) {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_KEYWORD_PUNC_EQUAL,
            blasphemy::blasphemy_type::SYNTAX,
            tokens.empty() ? blasphemy::NO_CODE_POS : tokens.front()->get_code_position()
        );

        return false;
    }

    return true;
}
