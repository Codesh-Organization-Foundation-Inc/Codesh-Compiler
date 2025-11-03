#include "import_parser.h"

#include "ast/import_declaration_ast_node.h"
#include "util.h"

namespace ast = codesh::ast;

std::unique_ptr<ast::import_declaration_ast_node> codesh::parser::parse_import(std::queue<std::unique_ptr<token>> &tokens)
{
    tokens.pop();
    if (tokens.empty())
    {
        throw std::runtime_error("Expected identifier"); //TODO: Convert to custom Codesh error
    }

    std::unique_ptr<ast::import_declaration_ast_node> import_node = std::make_unique<ast::import_declaration_ast_node>();

    // Check if is a static import
    switch (tokens.front()->get_group())
    {
    case token_group::KEYWORD_IMPORT_STATIC:
        import_node->set_is_static(true);
        break;

    case token_group::KEYWORD_IMPORT_REGULAR:
        // Already not static
        break;

    default: throw std::runtime_error("Unexpected token: Expected import type");
    }


    util::parse_fqcn(tokens, import_node->get_package_name());

    if (import_node->get_package_name().back() == "*")
    {
        import_node->get_package_name().pop_back();
        import_node->set_is_on_demand(true);
    }

    util::ensure_end_op(tokens);
    return import_node;
}
