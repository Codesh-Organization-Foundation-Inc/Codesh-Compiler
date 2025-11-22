#include "compilation_unit_parser.h"

#include "../token/token.h"
#include "../token/token_group.h"
#include "ast/compilation_unit_ast_node.h"
#include "util.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static codesh::definition::basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens);
static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
                                 ast::compilation_unit_ast_node *root_node);


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse_compilation_unit(
        std::queue<std::unique_ptr<token>> &tokens, const std::string &source_stem)
{
    std::unique_ptr<ast::compilation_unit_ast_node> node = std::make_unique<ast::compilation_unit_ast_node>(
        get_basad_type(tokens), source_stem
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


static codesh::definition::basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    switch (parser::util::consume_token(tokens)->get_group())
    {
    case codesh::token_group::KEYWORD_BASAD: return codesh::definition::basad_type::BASAD;
    case codesh::token_group::KEYWORD_BH: return codesh::definition::basad_type::BH;
    case codesh::token_group::KEYWORD_IAW: return codesh::definition::basad_type::IAW;

    default: throw std::runtime_error("Unexpected token: Expected BASAD declaration"); //TODO: Convert to custom Codesh error
    }
}

static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
                                 ast::compilation_unit_ast_node *root_node)
{
    tokens.pop();
    parser::util::parse_fqcn(tokens, root_node->get_package_name());
}