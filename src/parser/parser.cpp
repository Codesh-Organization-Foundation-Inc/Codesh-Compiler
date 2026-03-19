#include "parser.h"

#include <iostream>

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "compilation_unit_parser.h"
#include "import_parser.h"
#include "parser/type/type_parser.h"
#include "util.h"

namespace ast = codesh::ast;


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse(std::queue<std::unique_ptr<token>> &tokens,
        const size_t file_id)
{
    if (tokens.empty())
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_BASAD,
            blasphemy::blasphemy_type::LEXICAL,
            lexer::NO_CODE_POS,
            true
        );
    }


    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens, file_id);

    if (root_node->get_basad_type() == definition::basad_type::IAW ||
        root_node->get_basad_type() == definition::basad_type::JCIK)
    {
        std::cout << "צַדִּיק בֶּאֱמוּנָתוֹ יִחְיֶה" << std::endl;
        std::exit(0);
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
            blasphemy::get_blasphemy_collector().add_blasphemy(
                //TODO: Move to formattable details
                tokens.empty() ? blasphemy::details::UNEXPECTED_TOKEN : fmt::format(
                    "{}: {}",
                    blasphemy::details::UNEXPECTED_TOKEN,
                    util::get_token_display_name(*tokens.front())
                ),
                blasphemy::blasphemy_type::SYNTAX,
                tokens.front()->get_code_position()
            );
            tokens.pop();
            break;
        }
    }


    return root_node;
}
