#include "builtins.h"

#include "blasphemy/blasphemy_collector.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country);
static void add_alias_labubu(codesh::semantic_analyzer::country_symbol &country);


void codesh::semantic_analyzer::builtins::collect_builtins(const symbol_table &table)
{
    //TODO: Properly wrap in Talmud Codesh country
    country_symbol &country = table.get_global_scope();

    add_alias_ktuvim(country);
    add_alias_labubu(country);
}

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    country.get_scope().add_symbol(
        codesh::semantic_analyzer::builtins::ALIAS_STRING,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            codesh::definition::fully_qualified_name::parse("java/lang/String", codesh::blasphemy::NO_CODE_POS),
            nullptr,
            std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>>(),
            std::move(attributes),
            nullptr
        )
    );
}

static void add_alias_labubu(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    country.get_scope().add_symbol(
        codesh::semantic_analyzer::builtins::ALIAS_OBJECT,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            codesh::definition::fully_qualified_name::parse("java/lang/Object", codesh::blasphemy::NO_CODE_POS),
            nullptr,
            std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>>(),
            std::move(attributes),
            nullptr
        )
    );
}
