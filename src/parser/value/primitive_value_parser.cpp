#include "primitive_value_parser.h"

#include "defenition/primitive_type.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "parser/ast/var_reference/error_value_ast_node.h"
#include "parser/ast/var_reference/evaluable_ast_node.h"
#include "parser/ast/var_reference/variable_reference_ast_node.h"
#include "parser/util.h"
#include "token/token.h"
#include "token/token_group.h"

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


std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_primitive_value(
        std::queue<std::unique_ptr<token>> &tokens)
{
    std::unique_ptr<ast::var_reference::value_ast_node> eval_ast_node;

    switch (tokens.front()->get_group())
    {
    case token_group::KEYWORD_THIS:
    case token_group::IDENTIFIER: {
        auto id_pos = tokens.front()->get_code_position();

        definition::fully_qualified_name value;
        util::parse_this_and_fqn(tokens, value);

        eval_ast_node = std::make_unique<variable_reference_ast_node>(id_pos, value);

        break;
    }

    case token_group::LITERAL_STRING: {
        auto str_pos = tokens.front()->get_code_position();

        eval_ast_node = std::make_unique<ast::var_reference::evaluable_ast_node<std::string>>(
            str_pos,
            std::make_unique<ast::type::custom_type_ast_node>(str_pos, "java/lang/String"),
            util::consume_alnum_identifier_token(tokens)->get_content()
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

    default: {
        eval_ast_node = std::make_unique<ast::var_reference::error_value_ast_node>(tokens.front()->get_code_position());
    }
    }

    return eval_ast_node;
}


template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        const std::function<T(const std::string &)> &mapper)
{
    auto pos = tokens.front()->get_code_position();

    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        pos,
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(pos, primitive_type),
        mapper(
            codesh::parser::util::consume_alnum_identifier_token(tokens)->get_content()
        )
    );
}

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        T value)
{
    auto pos = codesh::parser::util::consume_token(tokens)->get_code_position();

    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        pos,
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(pos, primitive_type),
        std::move(value)
    );
}

static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<bool>> make_bool_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        const bool value)
{
    return make_evaluable<bool>(
        tokens,
        codesh::definition::primitive_type::BOOLEAN,
        value
    );
}