#include "compilation_unit_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "token/token.h"
#include "token/token_group.h"
#include "util.h"

namespace ast = codesh::ast;

static codesh::definition::basad_type parse_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens);


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse_compilation_unit(
        std::queue<std::unique_ptr<token>> &tokens, const size_t file_id)
{
    auto node = std::make_unique<ast::compilation_unit_ast_node>(
        file_id, parse_basad_type(tokens)
    );

    if (!tokens.empty())
    {
        if (tokens.front()->get_group() == token_group::KEYWORD_ORIGIN_COUNTRY)
        {
            tokens.pop();
            util::parse_fqn(tokens, node->get_package_name());
            util::ensure_end_op(tokens);
        }
    }

    return node;
}

static codesh::definition::basad_type parse_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    switch (codesh::parser::util::consume_token(tokens, codesh::blasphemy::details::NO_BASAD)->get_group())
    {
    case codesh::token_group::KEYWORD_BASAD: return codesh::definition::basad_type::BASAD;
    case codesh::token_group::KEYWORD_BH: return codesh::definition::basad_type::BH;
    case codesh::token_group::KEYWORD_IAW: return codesh::definition::basad_type::IAW;

    default: {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_BASAD,
            codesh::blasphemy::blasphemy_type::LEXICAL,
            {1, 0},
            true
        );

        return codesh::definition::basad_type::MISSING;
    }
    }
}
