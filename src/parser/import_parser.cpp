#include "import_parser.h"

#include "ast/import_declaration_ast_node.h"
#include "util.h"

namespace ast = codesh::ast;
namespace parser = codesh::parser;

std::unique_ptr<ast::import_declaration_ast_node> parse_import(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    tokens.pop();
    if (tokens.empty())
    {
        throw std::runtime_error("Expected identifier"); //TODO: Convert to custom Codesh error
    }

    std::unique_ptr<ast::import_declaration_ast_node> import_node = std::make_unique<ast::import_declaration_ast_node>();

    if (tokens.front()->get_group() == codesh::token_group::KEYWORD_IMPORT_STATIC)
    {
        import_node->set_is_static(true);
    }

    parser::util::parse_fqcn(tokens, import_node->get_package_name());

    if (import_node->get_package_name().back() == "*")
    {
        import_node->get_package_name().pop_back();
        import_node->set_is_on_demand(true);
    }

    parser::util::ensure_end_op(tokens);
    return import_node;
}
