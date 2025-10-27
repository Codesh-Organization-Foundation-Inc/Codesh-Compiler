#include "parser.h"

#include "ast/compilation_unit_ast_node.h"
#include "ast/type/attributes_ast_node.h"
#include "ast/type/class_declaration_ast_node.h"

namespace codesh::ast::type
{
class attributes_ast_node;
}
namespace ast = codesh::ast;

static codesh::definition::basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::compilation_unit_ast_node> parse_compilation_unit(std::queue<std::unique_ptr<codesh::token>> &tokens);
static void parse_fqcn(std::queue<std::unique_ptr<codesh::token>> &tokens, std::list<std::string> &fqcn);
static void parse_origin_country(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::compilation_unit_ast_node *root_node);
static void ensure_end_op(std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::type::type_declaration_ast_node> parse_type_declaration(
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::type::class_declaration_ast_node> parse_class_declaration(
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::type::attributes_ast_node> parse_attributes(
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static bool consuming_check(std::queue<std::unique_ptr<codesh::token>> &tokens, codesh::token_group token_group);
static void ensure_tokens_exist(const std::queue<std::unique_ptr<codesh::token>> &tokens);
static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type::class_declaration_ast_node *const class_node);

/**
 * Pops the latest token from the queue and returns it, transferring its ownership to the caller.
 * @return The consumed token
 */
static std::unique_ptr<codesh::token> consume_token(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    //TODO: Request custom error message
    ensure_tokens_exist(tokens);

    std::unique_ptr<codesh::token> token = std::move(tokens.front());
    tokens.pop();
    return token;
}



std::unique_ptr<ast::impl::ast_node> codesh::parse(std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty())
        throw std::runtime_error("Missing BASAD declaration"); //TODO: Convert to custom Codesh error


    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens);

    if (root_node->get_basad_type() == definition::basad_type::IAW)
    {
        //TODO: Return the joke program
        return root_node;
    }

    // Global scope
    while (!tokens.empty())
    {
        switch (tokens.front()->get_group())
        {
        case token_group::KEYWORD_LET:
            root_node->get_type_declarations().push_back(parse_type_declaration(tokens));
            break;

        default: throw std::runtime_error("Unexpected token: Expected ויהי"); //TODO: Convert to custom Codesh error
        }
    }


    return root_node;
}



static std::unique_ptr<ast::type::type_declaration_ast_node> parse_type_declaration(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    tokens.pop();

    switch (consume_token(tokens)->get_group())
    {
    case codesh::token_group::KEYWORD_CLASS: return parse_class_declaration(tokens);
    case codesh::token_group::KEYWORD_INTERFACE:; //TODO
    case codesh::token_group::KEYWORD_ENUM:; //TODO
    case codesh::token_group::KEYWORD_ANNOTATION: return nullptr; //TODO

    default: throw std::runtime_error("Unexpected token: Expected class, interface, enum or annotation");
    }
}


static std::unique_ptr<ast::type::class_declaration_ast_node> parse_class_declaration(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
    {
        throw std::runtime_error("Unexpected token: Expected ושמו");
    }


    // Get name
    const std::unique_ptr<codesh::token> name_token = consume_token(tokens);

    if (name_token->get_group() != codesh::token_group::IDENTIFIER)
    {
        throw std::runtime_error("Unexpected token: Expected identifier");
    }


    std::unique_ptr<ast::type::class_declaration_ast_node> node = std::make_unique<ast::type::class_declaration_ast_node>(
        static_cast<const codesh::identifier_token *>(name_token.get())->get_content() // NOLINT(*-pro-type-static-cast-downcast)
    );


    // Get attributes
    node->set_attributes(parse_attributes(tokens));


    // Start scope
    if (consume_token(tokens)->get_group() != codesh::token_group::SCOPE_BEGIN)
    {
        throw std::runtime_error("Unexpected token: Expected start of scope (ויאמר:)");
    }

    parse_class_scope(tokens, node.get());


    return node;
}

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type::class_declaration_ast_node *const class_node)
{
    while (!tokens.empty())
    {
        switch (consume_token(tokens)->get_group())
        {
        //TODO: Parse class scope

        case codesh::token_group::SCOPE_END: return;
        default: throw std::runtime_error("Unexpected token");
        }
    }

    throw std::runtime_error("Unexpected EOF: Expected end of scope (ויתם:)");
}


static std::unique_ptr<ast::type::attributes_ast_node> parse_attributes(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    std::unique_ptr<ast::type::attributes_ast_node> node = std::make_unique<ast::type::attributes_ast_node>();

    // Attributes are optional, so check whether they exist at all.
    if (tokens.empty() || tokens.front()->get_group() == codesh::token_group::SCOPE_BEGIN)
        return node;


    // Optional 1: Visibility
    if (const auto visibility = codesh::definition::token_group_to_visibility(tokens.front().get()))
    {
        node->set_visibility(visibility.value());
        tokens.pop();
    }

    // Optional 2: Static
    if (consuming_check(tokens, codesh::token_group::KEYWORD_STATIC))
    {
        node->set_is_static(true);
    }

    // Optional 3: Abstract
    if (consuming_check(tokens, codesh::token_group::KEYWORD_ABSTRACT))
    {
        node->set_is_abstract(true);
    }

    // Optional 4: Final
    if (consuming_check(tokens, codesh::token_group::KEYWORD_FINAL))
    {
        node->set_is_final(true);
    }


    if (!consuming_check(tokens, codesh::token_group::KEYWORD_SHALL_BE))
    {
        // If the last keyword wasn't Shall Be, it means that the user entered a nonsensical keyword before,
        // or did not close the attribute statement with Shall Be.
        throw std::runtime_error("Unexpected token: Expected attribute list enclosed by היה");
    }

    return node;
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


static codesh::definition::basad_type get_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    switch (consume_token(tokens)->get_group())
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
        throw std::runtime_error("Unexpected token: Expected colon"); //TODO: Convert to custom Codesh error
    }

    tokens.pop();
}

/**
 * Checks whether the group of the first token matches the requested one.
 * If so, pops it from the queue.
 * @return Whether the token group matches the requested
 */
static bool consuming_check(std::queue<std::unique_ptr<codesh::token>> &tokens, const codesh::token_group token_group)
{
    if (!tokens.empty() && tokens.front()->get_group() != token_group)
        return false;

    tokens.pop();
    return true;
}

static void ensure_tokens_exist(const std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (tokens.empty())
    {
        throw std::runtime_error("Unexpected EOF");
    }
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
            throw std::runtime_error("Unexpected token: Expected identifier");
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
