#include "new_value_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/method/operation/new_array_ast_node.h"
#include "parser/ast/method/operation/new_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/type/class/method_parser.h"
#include "parser/util.h"
#include "value_parser.h"

static std::unique_ptr<codesh::ast::op::new_array_ast_node> parse_array_initialization(
        std::queue<std::unique_ptr<codesh::token>> &tokens, std::unique_ptr<codesh::ast::type::type_ast_node> type,
        codesh::lexer::code_position new_pos);


std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_new_operator(
        std::queue<std::unique_ptr<token>> &tokens)
{
    const auto new_pos = util::consume_token(tokens)->get_code_position();

    auto parsed_type = util::parse_type(tokens);

    if (parsed_type->get_array_dimensions() != 0)
        return parse_array_initialization(tokens, std::move(parsed_type), new_pos);


    const auto *custom_type = dynamic_cast<ast::type::custom_type_ast_node *>(parsed_type.get());
    if (!custom_type)
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::INVALID_TYPE_FOR_NEW,
            blasphemy::blasphemy_type::SEMANTIC,
            new_pos
        );

        return nullptr;
    }

    auto constructed_type = std::unique_ptr<ast::type::custom_type_ast_node>(
        static_cast<ast::type::custom_type_ast_node *>(parsed_type.release()) // NOLINT(*-pro-type-static-cast-downcast)
    );

    auto new_node = std::make_unique<ast::op::new_ast_node>(
        new_pos,
        std::move(constructed_type)
    );

    if (util::consuming_check(tokens, token_group::OPEN_PARENTHESIS))
    {
        parse_methods_call_parameters(tokens, *new_node);
    }

    return new_node;
}

static std::unique_ptr<codesh::ast::op::new_array_ast_node> parse_array_initialization(
        std::queue<std::unique_ptr<codesh::token>> &tokens, std::unique_ptr<codesh::ast::type::type_ast_node> type,
        const codesh::lexer::code_position new_pos)
{
    auto array_node = std::make_unique<codesh::ast::op::new_array_ast_node>(
        new_pos,
        std::move(type)
    );

    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_LENGTH))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_HIS_LENGTH,
            codesh::blasphemy::blasphemy_type::SEMANTIC,
            new_pos
        );
    }

    do
    {
        array_node->add_dimension(codesh::parser::value::parse_value(tokens));
    }
    while (codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_BY));

    if (!codesh::parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_CUBIT))
    {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_KEYWORD_CUBITS,
            codesh::blasphemy::blasphemy_type::SEMANTIC,
            new_pos
        );
    }

    return array_node;
}
