#include "compilation_unit_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "token/token.h"
#include "token/token_group.h"
#include "util.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
                                 ast::compilation_unit_ast_node *root_node);


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse_compilation_unit(
        std::queue<std::unique_ptr<token>> &tokens, const std::string &source_stem)
{
    std::unique_ptr<ast::compilation_unit_ast_node> node = std::make_unique<ast::compilation_unit_ast_node>(
        source_stem
    );

    if (!tokens.empty())
    {
        if (tokens.front()->get_group() == token_group::KEYWORD_ORIGIN_COUNTRY)
        {
            parse_origin_country(tokens, node.get());
        }
    }

    return node;
}

static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
                                 ast::compilation_unit_ast_node *root_node)
{
    tokens.pop();
    parser::util::parse_fqcn(tokens, root_node->get_package_name());
    parser::util::ensure_end_op(tokens);
}