#include "method_parser.h"

#include "../../ast/method/method_declaration_ast_node.h"
#include "../../ast/method/operation/method_call_ast_node.h"
#include "../../../blasphemies/blasphemy_collector.h"
#include "../../../blasphemies/blasphemy_details.h"
#include "../../util.h"

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call);

void codesh::parser::parse_method(std::queue<std::unique_ptr<token>> &tokens,
    ast::method::method_declaration_ast_node &method_decl)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_FUNCTION_CALL:
            method_decl.get_body().push_back(parse_methods_call(tokens));
            break;

        case token_group::KEYWORD_LET:
            method_decl.get_parameters().push_back(parse_variable_declaration(tokens));
            break;

        case token_group::SCOPE_END:
            tokens.pop();
            return;

        default: error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::UNEXPECTED_TOKEN,
            error::blasphemy_type::SYNTAX);
        }
    }

    error::get_blasphemy_collector().add_blasphemy(error::blasphemy_details::NO_SCOPE_END,
        error::blasphemy_type::SYNTAX);
}

std::unique_ptr<codesh::ast::method::operation::method_call_ast_node> codesh::parser::parse_methods_call(
    std::queue<std::unique_ptr<token>> &tokens)
{
    tokens.pop();

    auto method_call_node = std::make_unique<ast::method::operation::method_call_ast_node>();

    util::parse_fqcn(tokens, method_call_node->get_fqcn());

    if (util::consuming_check(tokens, token_group::OPEN_PARENTHESIS))
    {
        parse_methods_call_parameters(tokens, *method_call_node);
    }

    util::ensure_end_op(tokens);
    return method_call_node;
}

std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> parse_variable_declaration(
    std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    //ויהי שלם ושמו ב ויהי 1:
    //  ויהי שלם ושמו הבא:
    tokens.pop();

    auto variable_decl_ast_node_type = codesh::parser::util::parse_type(tokens);

    auto variable_decl_ast_node = std::make_unique<codesh::ast::local_variable_declaration_ast_node>();

    variable_decl_ast_node->set_type(std::move(variable_decl_ast_node_type));

    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_NAME))
    {
        codesh::error::get_blasphemy_collector().add_blasphemy(codesh::error::blasphemy_details::NO_KEYWORD_NAME,
            codesh::error::blasphemy_type::SYNTAX);
    }

    const auto name_token = codesh::parser::util::consume_identifier_token(tokens);

    variable_decl_ast_node->set_name(name_token->get_content());

    if (tokens.front()->get_group() == codesh::token_group::KEYWORD_LET)
    {
        // How do I check if 1 is int and "bob" is str
    }

    codesh::parser::util::ensure_end_op(tokens);
    return variable_decl_ast_node;
}

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    //TODO: Convert the following to utilize util::peeking_check (once blasphemies is merged: !28)
    while (tokens.front()->get_group() != codesh::token_group::CLOSE_PARENTHESIS)
    {
        method_call.get_arguments().push_back(codesh::parser::util::parse_value(tokens));

        if (!tokens.empty() && tokens.front()->get_group() == codesh::token_group::PUNCTUATION_ARG_SEPARATOR ||
            tokens.front()->get_group() == codesh::token_group::CLOSE_PARENTHESIS)
        {
            tokens.pop();
        }
        else
        {
            throw std::runtime_error("Unexpected token"); //TODO: Convert to custom Codesh error
        }
    }
}

