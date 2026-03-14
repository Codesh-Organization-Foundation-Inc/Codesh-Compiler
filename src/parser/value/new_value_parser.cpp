#include "new_value_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/method/operation/new_ast_node.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/type/class/method_parser.h"
#include "parser/util.h"

std::unique_ptr<codesh::ast::var_reference::value_ast_node> codesh::parser::value::parse_new_operator(
        std::queue<std::unique_ptr<token>> &tokens)
{
    const auto new_pos = util::consume_token(tokens)->get_code_position();

    auto parsed_type = util::parse_type(tokens);
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
