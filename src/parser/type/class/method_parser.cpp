#include "method_parser.h"

#include "../../ast/method/operation/method_call_ast_node.h"
#include "../../ast/type/custom_type_ast_node.h"
#include "../../ast/type/primitive_type_ast_node.h"
#include "../../ast/var_reference/evaluable_ast_node.h"
#include "../../util.h"

#include <functional>

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call);

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


void codesh::parser::parse_method(std::queue<std::unique_ptr<token>> &tokens)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_FUNCTION_CALL:
            parse_methods_call(tokens);

        case token_group::SCOPE_END:
            tokens.pop();
            return;

        default: throw std::runtime_error("Unexpected token");
        }
    }
}


void codesh::parser::parse_methods_call(std::queue<std::unique_ptr<token>> &tokens)
{
    const auto method_call_node = std::make_unique<ast::method::operation::method_call_ast_node>();

    util::parse_fqcn(tokens, method_call_node->get_fqcn());

    if (util::consuming_check(tokens, token_group::OPEN_PARENTHESIS))
    {

        parse_methods_call_parameters(tokens, *method_call_node);
    }
}

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    while (tokens.front()->get_group() != codesh::token_group::CLOSE_PARENTHESIS)
    {
        std::unique_ptr<codesh::ast::var_reference::value_ast_node> eval_ast_node;

        switch (tokens.front()->get_group())
        {

        case codesh::token_group::IDENTIFIER: {
            eval_ast_node = std::make_unique<codesh::ast::var_reference::value_ast_node>(
                std::make_unique<codesh::ast::type::custom_type_ast_node>(
                    codesh::parser::util::consume_identifier_token(tokens)->get_content()
                )
            );

            break;
        }
        case codesh::token_group::LITERAL_NUMBER_INT: {
            eval_ast_node = make_evaluable<int>(
                tokens,
                codesh::definition::primitive_type::INTEGER,

                [](const std::string &content) {
                    return std::stoi(content);
                }
            );

            break;
        }
        case codesh::token_group::LITERAL_NUMBER_FLOAT: {
            eval_ast_node = make_evaluable<float>(
                tokens,
                codesh::definition::primitive_type::FLOAT,

                [](const std::string &content) {
                    return std::stof(content);
                }
            );

            break;
        }
        case codesh::token_group::LITERAL_NUMBER_DOUBLE: {
            eval_ast_node = make_evaluable<double>(
                tokens,
                codesh::definition::primitive_type::DOUBLE,

                [](const std::string &content) {
                    return std::stod(content);
                }
            );

            break;
        }
        case codesh::token_group::LITERAL_CHAR: {
            eval_ast_node = make_evaluable<char>(
                tokens,
                codesh::definition::primitive_type::CHAR,

                [](const std::string &content) {
                    return content[0];
                }
            );

            break;
        }

        case codesh::token_group::KEYWORD_TRUE: {
            eval_ast_node = make_bool_evaluable(
                tokens,
                true
            );

            break;
        }
        case codesh::token_group::KEYWORD_FALSE: {
            eval_ast_node = make_bool_evaluable(
                tokens,
                false
            );

            break;
        }

        }

        method_call.get_arguments().push_back(std::move(eval_ast_node));
    }

}

template <typename T>
static std::unique_ptr<codesh::ast::var_reference::evaluable_ast_node<T>> make_evaluable(
        std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::definition::primitive_type primitive_type,
        const std::function<T(const std::string &)> &mapper)
{
    return std::make_unique<codesh::ast::var_reference::evaluable_ast_node<T>>(
        std::make_unique<codesh::ast::type::primitive_type_ast_node>(primitive_type),
        mapper(codesh::parser::util::consume_alnum_identifier_token(tokens)->get_content())
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
