#include "parser.h"

#include "compilation_unit_parser.h"
#include "import_parser.h"
#include "type/type_parser.h"
#include "util.h"

namespace ast = codesh::ast;


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse(std::queue<std::unique_ptr<token>> &tokens,
        const std::string &source_stem)
{
    if (tokens.empty())
        throw std::runtime_error("Missing BASAD declaration"); //TODO: Convert to custom Codesh error

    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens, source_stem);

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
