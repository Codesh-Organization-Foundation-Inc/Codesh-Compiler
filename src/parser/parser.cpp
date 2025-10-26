#include "parser.h"

#include "ast/compilation_unit_ast_node.h"

namespace ast = codesh::ast;

static basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::compilation_unit_ast_node> parse_compilation_unit(std::queue<std::unique_ptr<codesh::token>> &tokens);
static void parse_fqcn(std::queue<std::unique_ptr<codesh::token>> &tokens, std::list<std::string> &fqcn);
static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::compilation_unit_ast_node *root_node);
static void ensure_end_op(std::queue<std::unique_ptr<codesh::token>> &tokens);

/**
 * Pops the latest token from the queue and returns it, transferring its ownership to the caller.
 * @return The consumed token
 */
static std::unique_ptr<codesh::token> consume_token(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    std::unique_ptr<codesh::token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}



std::unique_ptr<ast::impl::ast_node> codesh::parse(std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty())
        throw std::runtime_error("Missing BASAD declaration"); //TODO: Convert to custom Codesh error


    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens);

    if (root_node->get_basad_type() == basad_type::IAW)
    {
        //TODO: Return the joke program
        return root_node;
    }


    return root_node;
}


static std::unique_ptr<ast::compilation_unit_ast_node> parse_compilation_unit(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    std::unique_ptr<ast::compilation_unit_ast_node> node = std::make_unique<ast::compilation_unit_ast_node>(
        get_basad_type(tokens)
    );

    if (!tokens.empty())
    {
        if (tokens.front()->get_group() == codesh::token_group::KEYWORD_ORIGIN_COUNTRY)
        {
            parse_origin_country(tokens, node.get());
        }
    }

    return node;
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

static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::compilation_unit_ast_node *root_node)
{
    tokens.pop();
    parse_fqcn(tokens, root_node->get_package_name());
    ensure_end_op(tokens);
}


/**
 * Ensures a colon exists at the current token, and consumes it.
 */
static void ensure_end_op(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (tokens.empty() || tokens.front()->get_group() != codesh::token_group::PUNCTUATION_END_OP)
    {
        throw std::runtime_error("Expected colon"); //TODO: Convert to custom Codesh error
    }

    tokens.pop();
}

/**
 * Parses a Fully Qualified Class Name
 */
static void parse_fqcn(std::queue<std::unique_ptr<codesh::token>> &tokens, std::list<std::string> &fqcn)
{
    while (!tokens.empty())
    {
        std::unique_ptr<codesh::token> id = consume_token(tokens);

        if (id->get_group() != codesh::token_group::IDENTIFIER)
        {
            throw std::runtime_error("Expected identifier in package name");
        }

        fqcn.push_back(static_cast<codesh::identifier_token *>(id.get())->get_content()); // NOLINT(*-pro-type-static-cast-downcast)

        if (tokens.empty() || tokens.front()->get_group() != codesh::token_group::PUNCTUATION_DOT)
        {
            break;
        }

        // Consume the dot
        tokens.pop();
    }

}
