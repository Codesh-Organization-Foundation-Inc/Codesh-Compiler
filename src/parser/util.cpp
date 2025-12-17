#include "util.h"

#include "../blasphemy/blasphemy_collector.h"
#include "../blasphemy/details.h"
#include "../defenition/fully_qualified_class_name.h"
#include "ast/type/custom_type_ast_node.h"
#include "ast/type/primitive_type_ast_node.h"
#include "ast/var_reference/evaluable_ast_node.h"

#include <functional>

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        const std::function<T(const std::string &)> &mapper);

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        T value);

static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<bool>> make_bool_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        bool value);

static std::unique_ptr<codesh::identifier_token> make_error_identifier_token();

/**
 * The identifier used when there was an error parsing an identifier
 */
static const std::string ERROR_IDENTIFIER_CONTENT = "סְדוֹם וַעֲמֹרָה";


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
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<token> token = consume_token(tokens, blasphemy::details::NO_IDENTIFIER);

    if (token->get_group() != token_group::IDENTIFIER)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IDENTIFIER,
            blasphemy::blasphemy_type::SYNTAX, std::nullopt);

        return make_error_identifier_token();
    }

    return std::unique_ptr<identifier_token>(
        static_cast<identifier_token *>(token.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );
}

std::unique_ptr<codesh::identifier_token> codesh::parser::util::consume_alnum_identifier_token(
        std::queue<std::unique_ptr<token>> &tokens, const std::string &no_tokens_blasphemy_details)
{
    std::unique_ptr<token> token = consume_token(tokens, no_tokens_blasphemy_details);

    if (token::get_token_type(token->get_group()) != token_type::IDENTIFIER)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            no_tokens_blasphemy_details,
            blasphemy::blasphemy_type::SYNTAX,
            std::nullopt
        );

        return make_error_identifier_token();
    }

    return std::unique_ptr<identifier_token>(
        static_cast<identifier_token *>(token.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );
}

static std::unique_ptr<codesh::identifier_token> make_error_identifier_token()
{
    return std::make_unique<codesh::identifier_token>(codesh::token_group::IDENTIFIER, ERROR_IDENTIFIER_CONTENT);
}


std::unique_ptr<codesh::ast::type::type_ast_node> codesh::parser::util::parse_type(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::type::type_ast_node> result;

    const auto type_token = consume_token(tokens, blasphemy::details::NO_TYPE);

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
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_TYPE,
            blasphemy::blasphemy_type::SYNTAX, std::nullopt);
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
        blasphemy::get_blasphemy_collector().add_blasphemy(no_tokens_blasphemy_details,
            blasphemy::blasphemy_type::SYNTAX, std::nullopt, true);
    }
}

void codesh::parser::util::ensure_end_op(std::queue<std::unique_ptr<token>> &tokens)
{
    if (!consuming_check(tokens, token_group::PUNCTUATION_END_OP))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_PUNCTUATION_END_OP,
            blasphemy::blasphemy_type::SYNTAX);
        return;
    }
}


void codesh::parser::util::parse_fqcn(std::queue<std::unique_ptr<token>> &tokens, definition::fully_qualified_class_name &fqcn_out)
{
    while (!tokens.empty())
    {
        std::unique_ptr<token> id = consume_token(tokens, blasphemy::details::NO_IDENTIFIER);

        if (id->get_group() != token_group::IDENTIFIER)
        {
            if (id->get_group() == token_group::PUNCTUATION_WILDCARD)
            {
                fqcn_out.set_is_wildcard(true);
            }
            else
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IDENTIFIER,
                    blasphemy::blasphemy_type::SYNTAX);
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
                blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IDENTIFIER,
                    blasphemy::blasphemy_type::SYNTAX);
            }
        }

        break;
    }
}



std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::util::parse_value(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::IDENTIFIER: {
        eval_ast_node = std::make_unique<ast::var_reference::value_ast_node>(
            std::make_unique<ast::type::custom_type_ast_node>(
                consume_identifier_token(tokens)->get_content()
            )
        );

        break;
    }

    case token_group::LITERAL_NUMBER_INT: {
        eval_ast_node = make_evaluable<int>(
            tokens, definition::primitive_type::INTEGER,

            [](const std::string &content) {
                return std::stoi(content);
            }
        );

        break;
    }

    case token_group::LITERAL_NUMBER_FLOAT: {
        eval_ast_node = make_evaluable<float>(
            tokens, definition::primitive_type::FLOAT,

            [](const std::string &content) {
                return std::stof(content);
            }
        );

        break;
    }

    case token_group::LITERAL_NUMBER_DOUBLE: {
        eval_ast_node = make_evaluable<double>(
            tokens, definition::primitive_type::DOUBLE,

            [](const std::string &content) {
                return std::stod(content);
            }
        );

        break;
    }

    case token_group::LITERAL_CHAR: {
        eval_ast_node = make_evaluable<char>(
            tokens, definition::primitive_type::CHAR,

            [](const std::string &content) {
                return content[0];
            }
        );

        break;
    }

    case token_group::KEYWORD_TRUE: {
        eval_ast_node = make_bool_evaluable(tokens, true);

        break;
    }

    case token_group::KEYWORD_FALSE: {
        eval_ast_node = make_bool_evaluable(tokens, false);

        break;
    }

    default:
        throw std::runtime_error("Unexpected token"); //TODO: Convert to custom Codesh error
    }

    return eval_ast_node;
}


template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        const std::function<T(const std::string &)> &mapper)
{
    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(primitive_type),
        mapper(
            codesh::parser::util::consume_alnum_identifier_token(
            tokens, "לא אמור לקרות"
        )->get_content())
    );
}

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        T value)
{
    tokens.pop();

    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(primitive_type),
        std::move(value)
    );
}

static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<bool>> make_bool_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const bool value)
{
    tokens.pop();

    return make_evaluable<bool>(
        tokens,
        codesh::definition::primitive_type::BOOLEAN,
        value
    );
}
