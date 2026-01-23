#include "parser.h"

#include "blasphemy/blasphemy_collector.h"
#include "blasphemy/details.h"
#include "compilation_unit_parser.h"
#include "import_parser.h"
#include "parser/type/type_parser.h"
#include "util.h"

namespace ast = codesh::ast;

static codesh::definition::basad_type parse_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens);


std::unique_ptr<ast::compilation_unit_ast_node> codesh::parser::parse(std::queue<std::unique_ptr<token>> &tokens,
        const std::string &source_stem)
{
    if (tokens.empty())
    {
        blasphemy::get_blasphemy_collector().add_blasphemy(
            blasphemy::details::NO_BASAD,
            blasphemy::blasphemy_type::LEXICAL,
            std::nullopt,
            true
        );
    }


    const auto basad_type = parse_basad_type(tokens);

    std::unique_ptr<ast::compilation_unit_ast_node> root_node = parse_compilation_unit(tokens, source_stem);

    if (basad_type == definition::basad_type::IAW)
    {
        //TODO: Return the joke program
        // (Do not even make the compilation unit yet)
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
            root_node->get_type_declarations().push_back(parse_type_declaration(tokens, basad_type));
            break;

        default:
            blasphemy::get_blasphemy_collector().add_blasphemy(blasphemy::details::NO_KEYWORD_SHALL_BE,
                blasphemy::blasphemy_type::SYNTAX);
            tokens.pop();
            break;
        }
    }


    return root_node;
}


static codesh::definition::basad_type parse_basad_type(std::queue<std::unique_ptr<codesh::token>> &tokens)
{
    switch (codesh::parser::util::consume_token(tokens, codesh::blasphemy::details::NO_BASAD)->get_group())
    {
    case codesh::token_group::KEYWORD_BASAD: return codesh::definition::basad_type::BASAD;
    case codesh::token_group::KEYWORD_BH: return codesh::definition::basad_type::BH;
    case codesh::token_group::KEYWORD_IAW: return codesh::definition::basad_type::IAW;

    default: {
        codesh::blasphemy::get_blasphemy_collector().add_blasphemy(
            codesh::blasphemy::details::NO_BASAD,
            codesh::blasphemy::blasphemy_type::LEXICAL,
            std::nullopt,
            true
        );

        return codesh::definition::basad_type::MISSING;
    }
    }
}
