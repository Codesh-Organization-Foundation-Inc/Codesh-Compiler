#include "parser.h"

#include "ast/compilation_unit_ast_node.h"

namespace ast = codesh::ast;

static std::unique_ptr<codesh::token> consume_token(std::queue<std::unique_ptr<codesh::token>> &tokens);
static basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens);
static void set_fqcn(std::queue<std::unique_ptr<codesh::token>> &tokens, std::list<std::string> &fqcn);



std::unique_ptr<ast::impl::ast_node> codesh::parse(std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty())
        throw std::runtime_error("Missing BASAD declaration"); //TODO: Convert to custom Codesh error


    std::unique_ptr<ast::compilation_unit_ast_node> root_node = std::make_unique<ast::compilation_unit_ast_node>(
        get_basad_type(tokens)
    );

    if (tokens.front().get()->get_group() == token_group::KEYWORD_ORIGIN_COUNTRY)
    {
        tokens.pop();
        set_fqcn(tokens, root_node->get_package_name());
    }

    return root_node;
}


static std::unique_ptr<codesh::token> consume_token(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    std::unique_ptr<codesh::token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}


static basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    switch (consume_token(tokens)->get_group())
    {
    case codesh::token_group::KEYWORD_BASAD: return basad_type::BASAD;
    case codesh::token_group::KEYWORD_BH: return basad_type::BH;
    case codesh::token_group::KEYWORD_IAW: return basad_type::IAW;
    default: throw std::runtime_error("Invalid keyword: Expected BASAD declaration"); //TODO: Convert to custom Codesh error
    }
}

/**
 * Parses a Fully Qualified Class Name
 */
static void set_fqcn(std::queue<std::unique_ptr<codesh::token>> &tokens, std::list<std::string> &fqcn)
{
    std::unique_ptr<codesh::token> identifier = consume_token(tokens);

    if (identifier->get_group() != codesh::token_group::IDENTIFIER)
    {
        throw std::runtime_error("Unexpected token"); // TODO: Convert to custom Codesh error
    }

    fqcn.push_back(static_cast<codesh::identifier_token *>(identifier.get())->get_content()); // NOLINT(*-pro-type-static-cast-downcast)

    // If more ל־'s exist, add more to the list.
    if (tokens.front()->get_group() == codesh::token_group::PUNCTUATION_DOT)
    {
        set_fqcn(tokens, fqcn);
    }
}
