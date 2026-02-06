#include "import_parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "parser/ast/import_declaration_ast_node.h"
#include "util.h"

namespace ast = codesh::ast;

std::unique_ptr<ast::import_declaration_ast_node> codesh::parser::parse_import(
        std::queue<std::unique_ptr<token>> &tokens)
{
    auto import_pos = tokens.front()->get_code_position();
    tokens.pop();

    std::unique_ptr<ast::import_declaration_ast_node> import_node = std::make_unique<ast::import_declaration_ast_node>(
        import_pos
    );

    // Check if is a static import
    util::ensure_tokens_exist(tokens, blasphemy::details::NO_IMPORT_TYPE); // TODO: provide better reason
    switch (tokens.front()->get_group())
    {
    case token_group::KEYWORD_IMPORT_STATIC:
        import_node->set_is_static(true);
        break;

    case token_group::KEYWORD_IMPORT_REGULAR:
        // Already not static
        break;

    default:
        blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_IMPORT_TYPE,
            blasphemy::blasphemy_type::SYNTAX, tokens.front()->get_code_position());

    }


    util::parse_fqn(tokens, import_node->get_package_name());

    if (import_node->get_package_name().is_wildcard())
    {
        import_node->set_is_on_demand(true);
    }

    util::ensure_end_op(tokens);
    return import_node;
}
