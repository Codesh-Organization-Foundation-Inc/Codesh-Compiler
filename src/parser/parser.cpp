#include "parser.h"

#include "compilation_unit_parser.h"
#include "import_parser.h"
#include "type/type_parser.h"
#include "util.h"
#include "../blasphemies/blasphemy_collector.h"

namespace ast = codesh::ast;


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse(std::queue<std::unique_ptr<token>> &tokens,
        const std::string &source_stem)
{
    if (tokens.empty())
    {
        error::get_blasphemy_collector().add_blasphemy(
            "בַּסַּ\"ד אֵינוֹ",
            error::blasphemy_type::LEXICAL,
            std::nullopt,
            true
        );
    }

    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens, source_stem);

    if (root_node->get_basad_type() == definition::basad_type::IAW)
    {
        //TODO: Return the joke program
        return root_node;
    }


    // Optionally parse imports
    while (util::peeking_check(tokens, token_group::KEYWORD_IMPORT))
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

        default:
            error::get_blasphemy_collector().add_blasphemy("נָבוֹא שְׁקָרַי: צֻּפָּה ויהי", error::blasphemy_type::SYNTAX);
            tokens.pop();
            break;
        }
    }


    return root_node;
}
