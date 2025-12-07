#include "method_parser.h"

#include "../../ast/method/operation/method_call_ast_node.h"
#include "../../util.h"

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
    auto method_call_node = std::make_unique<ast::method::operation::method_call_ast_node>();

    util::parse_fqcn(tokens, method_call_node->get_fqcn());

    if (!util::consuming_check(tokens, token_group::OPEN_PARENTHESIS))
    {
        throw std::runtime_error("Unexpected tokens while parsing method call"); //TODO change to custom Codesh error
    }



    if (!util::consuming_check(tokens, token_group::CLOSE_PARENTHESIS))
    {
        throw std::runtime_error("Unexpected tokens while parsing method call"); //TODO change to custom Codesh error
    }


}
