#include "method_parser.h"

#include "../../../blasphemy/blasphemy_collector.h"
#include "../../../blasphemy/details.h"
#include "../../ast/method/method_declaration_ast_node.h"
#include "../../ast/method/operation/method_call_ast_node.h"
#include "../../util.h"
#include "../type_parser.h"
#include "../../value_parser.h"

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call);

static void parse_if_statement(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope);

void codesh::parser::parse_method_scope(std::queue<std::unique_ptr<token>> &tokens,
        ast::method::method_scope_ast_node &method_scope)
{
    while (!tokens.empty())
    {
        //TODO: Implement
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_FUNCTION_CALL:
            method_scope.add_statement(parse_methods_call(tokens));
            break;

        case token_group::KEYWORD_LET:
            method_scope.add_local_variable(parse_variable_declaration(tokens));
            break;

        case token_group::OPERATOR_ADDITION:
        case token_group::OPERATOR_SUBTRACTION:
        case token_group::OPERATOR_MULTIPLICATION:
        case token_group::OPERATOR_DIVISION:
        case token_group::OPERATOR_MODULO:
            method_scope.add_statement(parse_value(tokens));
            if (!util::consuming_check(tokens, token_group::PUNCTUATION_END_OP))
            {
                blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_PUNCTUATION_END_OP,
                    blasphemy::blasphemy_type::SYNTAX);
            }
            break;
        case token_group::KEYWORD_IF:
            parse_if_statement(tokens, method_scope);
            break;

        case token_group::SCOPE_END:
            tokens.pop();
            method_scope.mark_end();
            return;

        default: blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::UNEXPECTED_TOKEN,
            blasphemy::blasphemy_type::SYNTAX);
            tokens.pop(); // TODO: remove it in the future if not needed
        }
    }

    blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_SCOPE_END,
        blasphemy::blasphemy_type::SYNTAX);
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

static void parse_if_statement(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::method_scope_ast_node &method_scope)
{
    tokens.pop();
    (tokens)



}
std::unique_ptr<codesh::ast::local_variable_declaration_ast_node> codesh::parser::parse_variable_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    tokens.pop();

    auto variable_decl_ast_node_type = util::parse_type(tokens);

    auto variable_decl_ast_node = std::make_unique<ast::local_variable_declaration_ast_node>();

    variable_decl_ast_node->set_type(std::move(variable_decl_ast_node_type));

    if (!util::consuming_check(tokens, token_group::KEYWORD_NAME))
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_NAME,
            blasphemy::blasphemy_type::SYNTAX);
    }

    const auto name_token = util::consume_identifier_token(tokens);

    variable_decl_ast_node->set_name(name_token->get_content());

    variable_decl_ast_node->set_attributes(parse_modifiers(tokens));

    if (tokens.front()->get_group() == token_group::KEYWORD_LET)
    {
        tokens.pop();
        variable_decl_ast_node->set_value(parse_value(tokens));
    }

    util::ensure_end_op(tokens);
    return variable_decl_ast_node;
}

static void parse_methods_call_parameters(std::queue<std::unique_ptr<codesh::token>> &tokens,
        codesh::ast::method::operation::method_call_ast_node &method_call)
{
    while (!codesh::parser::util::consuming_check(tokens, codesh::token_group::CLOSE_PARENTHESIS))
    {
        method_call.get_arguments().push_back(codesh::parser::parse_value(tokens));

        if (codesh::parser::util::consuming_check(tokens, codesh::token_group::PUNCTUATION_ARG_SEPARATOR))
            continue;

        // If there are no more arguments, there shouldn't be anything else besides a closing parenthesis.
        if (!codesh::parser::util::peeking_check(tokens, codesh::token_group::CLOSE_PARENTHESIS))
        {
            codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
                codesh::blasphemy::details::UNEXPECTED_TOKEN,
                codesh::blasphemy::blasphemy_type::SYNTAX
            );
        }
    }
}
