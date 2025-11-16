#include "class_parser.h"

#include "../ast/method_declaration_ast_node.h"
#include "../ast/type_declaration/class_declaration_ast_node.h"
#include "../ast/local_variable_declaration_ast_node.h"
#include "../util.h"
#include "../ast/type/primitive_type_ast_node.h"
#include "../ast/type/custom_type_ast_node.h"
#include "type_parser.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens);
static std::unique_ptr<ast::type::type_ast_node> parse_type(std::queue<std::unique_ptr<codesh::token>> &tokens);

static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *class_node);

static std::unique_ptr<ast::method_declaration_ast_node> parse_method_signature_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens);


std::unique_ptr<ast::type_decl::class_declaration_ast_node> codesh::parser::parse_class_declaration(
        std::queue<std::unique_ptr<token>> &tokens)
{
    if (util::consume_token(tokens)->get_group() != token_group::KEYWORD_NAME)
    {
        throw std::runtime_error("Unexpected token: Expected ושמו");
    }


    // Get name
    const std::unique_ptr<token> name_token = util::consume_identifier_token(tokens);

    std::unique_ptr<ast::type_decl::class_declaration_ast_node> node = std::make_unique<ast::type_decl::class_declaration_ast_node>(
        static_cast<const identifier_token *>(name_token.get())->get_content() // NOLINT(*-pro-type-static-cast-downcast)
    );


    // Get attributes
    node->set_attributes(parse_modifiers(tokens));


    // Start scope
    if (util::consume_token(tokens)->get_group() != token_group::SCOPE_BEGIN)
    {
        throw std::runtime_error("Unexpected token: Expected start of scope (ויאמר:)");
    }

    parse_class_scope(tokens, node.get());


    return node;
}


static void parse_class_scope(std::queue<std::unique_ptr<codesh::token>> &tokens,
        ast::type_decl::class_declaration_ast_node *const class_node)
{
    while (!tokens.empty())
    {
        switch (parser::util::consume_token(tokens)->get_group())
        {
        case codesh::token_group::KEYWORD_LET: {
            switch (tokens.front()->get_group())
            {
            case codesh::token_group::KEYWORD_METHOD:
            {
                class_node->get_methods().push_back(
                    parse_method_signature_scope(tokens)
                );
            }
            case codesh::token_group::KEYWORD_CLASS:
            {
                // TODO: Support ויהי עצם
                throw std::runtime_error("Inner classes not yet supported");
            }

            default:
                // Assume fields by default as they can either be custom types (identifiers)
                // or primitives (other tokens).
                parse_field_scope(tokens);
            }
        }

        case codesh::token_group::SCOPE_END:
            return;

        default: throw std::runtime_error("Unexpected token");
        }
    }
    throw std::runtime_error("Unexpected EOF: Expected end of scope (ויתם:)");
}

static void parse_field_scope(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    const codesh::token_group type_token = parser::util::consume_token(tokens)->get_group();


    if (type_token != codesh::token_group::IDENTIFIER)
    {
        //TODO: Parse token group as type and see if it is a primitive
        bool isPrimitive = false;

        if (!isPrimitive && type_token != codesh::token_group::KEYWORD_VAR)
        {
            throw std::runtime_error("Unexpected token");
        }
    }

    //TODO: Support fields
    throw std::runtime_error("Fields not yet supported");
}

static std::unique_ptr<ast::method_declaration_ast_node> parse_method_signature_scope(
        std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    tokens.pop();

    // ושמו
    if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
    {
        throw std::runtime_error("Unexpected token: Expected ושמו");
    }

    // * (the name)
    const std::unique_ptr<codesh::token> name_token = parser::util::consume_identifier_token(tokens);

    auto method_node = std::make_unique<ast::method_declaration_ast_node>();

    method_node->set_name(static_cast<const codesh::identifier_token *>(name_token.get())->get_content());

    // Get attributes
    method_node->set_attributes(parser::parse_modifiers(tokens));


    while (tokens.front()->get_group() == codesh::token_group::KEYWORD_TAKES)
    {
        tokens.pop();

        // Parse parameter type
        std::unique_ptr<ast::type::type_ast_node> param_type = parse_type(tokens);

        // ושמו
        if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::KEYWORD_NAME)
            throw std::runtime_error("Expected ושמו");

        // * (the name)
        std::unique_ptr<codesh::token> token_name = parser::util::consume_identifier_token(tokens);

        auto param = std::make_unique<ast::local_variable_declaration_ast_node>();
        param->set_type(std::move(param_type));
        param->set_name(
            static_cast<codesh::identifier_token *>(token_name.get())->get_content()
        );

        method_node->get_parameter_types().push_back(std::move(param));
    }


    // If no וישב, return type = void:
    if (tokens.front()->get_group() == codesh::token_group::KEYWORD_RETURN)
    {
        tokens.pop();
        method_node->set_return_type(parse_type(tokens));
    }
    else
    {
        method_node->set_return_type(
            std::make_unique<ast::type::primitive_type_ast_node>(
                codesh::definition::primitive_type::VOID
            )
        );
    }


    if (parser::util::consume_token(tokens)->get_group() != codesh::token_group::SCOPE_BEGIN)
    {
        throw std::runtime_error("Expected start of scope");
    }

    //TODO: Parse method scope

    return std::move(method_node);
}

static std::unique_ptr<ast::type::type_ast_node> parse_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    if (tokens.empty())
    {
        throw std::runtime_error("Unexpected EOF: expected type");
    }

    auto type_token = parser::util::consume_token(tokens);
    switch (type_token->get_group())
    {
    case codesh::token_group::KEYWORD_INTEGER:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::INTEGER);

    case codesh::token_group::KEYWORD_FLOAT:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::FLOAT);

    case codesh::token_group::KEYWORD_DOUBLE:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::DOUBLE);

    case codesh::token_group::KEYWORD_LONG:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::LONG);

    case codesh::token_group::KEYWORD_SHORT:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::SHORT);

    case codesh::token_group::KEYWORD_BYTE:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::BYTE);

    case codesh::token_group::KEYWORD_CHAR:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::CHAR);

    case codesh::token_group::KEYWORD_BOOLEAN:
        return std::make_unique<ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::BOOLEAN);

    case codesh::token_group::IDENTIFIER:
    {
        const std::string custom_name = dynamic_cast<codesh::identifier_token *>(type_token.get())->get_content();
        auto custom_type_node = std::make_unique<ast::type::custom_type_ast_node>();
        custom_type_node->set_name(custom_name);
        return custom_type_node;
    }

    default: // maybe its not needed
        throw std::runtime_error("Unexpected token: expected type");
    }
}
