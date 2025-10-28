#include "parser.h"

#include "ast/compilation_unit_ast_node.h"
#include "ast/type/attributes_ast_node.h"
#include "ast/type/class_declaration_ast_node.h"
#include "compilation_unit_parser.h"
#include "import_parser.h"
#include "util.h"

namespace codesh::ast::type
{
class attributes_ast_node;
}
namespace ast = codesh::ast;
namespace parser = codesh::parser;

static std::unique_ptr<ast::type::type_declaration_ast_node> parse_type_declaration(
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::type::class_declaration_ast_node> parse_class_declaration(
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::type::attributes_ast_node> parse_attributes(
        std::queue<std::unique_ptr<codesh::token>> &tokens);
static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type::class_declaration_ast_node *class_node);



std::unique_ptr<ast::impl::ast_node> codesh::parser::parse(std::queue<std::unique_ptr<token>> &tokens)
{
    if (tokens.empty())
        throw std::runtime_error("Missing BASAD declaration"); //TODO: Convert to custom Codesh error

    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens);

    if (root_node->get_basad_type() == definition::basad_type::IAW)
    {
        //TODO: Return the joke program
        return root_node;
    }


    // Optionally parse imports
    while (!tokens.empty() && tokens.front()->get_group() == token_group::KEYWORD_IMPORT)
    {
        root_node->get_import_declarations().push_back(parse_import(tokens));
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

    switch (parser::util::consume_token(tokens)->get_group())
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
    if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
    {
        throw std::runtime_error("Unexpected token: Expected ושמו");
    }


    // Get name
    const std::unique_ptr<codesh::token> name_token = parser::util::consume_token(tokens);

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
    if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::SCOPE_BEGIN)
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
        switch (parser::util::consume_token(tokens)->get_group())
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
    if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_STATIC))
    {
        node->set_is_static(true);
    }

    // Optional 3: Abstract
    if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_ABSTRACT))
    {
        node->set_is_abstract(true);
    }

    // Optional 4: Final
    if (parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_FINAL))
    {
        node->set_is_final(true);
    }


    if (!parser::util::consuming_check(tokens, codesh::token_group::KEYWORD_SHALL_BE))
    {
        // If the last keyword wasn't Shall Be, it means that the user entered a nonsensical keyword before,
        // or did not close the attribute statement with Shall Be.
        throw std::runtime_error("Unexpected token: Expected attribute list enclosed by היה");
    }

    return node;
}
