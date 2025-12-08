#include "method_parser.h"

#include "../../ast/method/operation/method_call_ast_node.h"
#include "../../util.h"

#include <functional>

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call);

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
        auto arg = codesh::parser::util::parse_value(tokens);
        method_call.get_arguments().push_back(std::move(arg));

        if ( !tokens.empty() && tokens.front()->get_group() == codesh::token_group::PUNCTUATION_ARG_SEPARATOR ||
            tokens.front()->get_group() == codesh::token_group::CLOSE_PARENTHESIS)
        {
            tokens.pop();
        }
        else
        {
            throw std::runtime_error("נְבוּא שִׁקְרִי");
        }

    }

}

