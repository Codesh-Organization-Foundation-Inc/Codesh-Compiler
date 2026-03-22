#include "builtins.h"

#include "lexer/source_file_info.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"

static codesh::semantic_analyzer::country_symbol &talmud_codesh_country(
        const codesh::semantic_analyzer::symbol_table &table);
static codesh::semantic_analyzer::country_symbol &nested_country(codesh::semantic_analyzer::country_symbol &parent,
        const std::string &name, const std::string &bin_fqn);

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country);
static void add_alias_labubu(codesh::semantic_analyzer::country_symbol &country);
static void add_alias_het(codesh::semantic_analyzer::country_symbol &country);


void codesh::semantic_analyzer::builtins::collect_builtins(const symbol_table &table)
{
    auto &country = talmud_codesh_country(table);

    add_alias_ktuvim(country);
    add_alias_labubu(country);
    add_alias_het(country);
}

static codesh::semantic_analyzer::country_symbol &talmud_codesh_country(
        const codesh::semantic_analyzer::symbol_table &table)
{
    codesh::semantic_analyzer::country_symbol &global = table.get_global_scope();
    codesh::semantic_analyzer::country_symbol &israel = nested_country(global, "ישראל", "ישראל");
    codesh::semantic_analyzer::country_symbol &codesh = nested_country(israel, "קודש","ישראל/קודש");
    codesh::semantic_analyzer::country_symbol &ben = nested_country(codesh, "בן", "ישראל/קודש/בן");
    codesh::semantic_analyzer::country_symbol &moshe = nested_country(ben, "משה", "ישראל/קודש/בן/משה");

    return moshe;
}

static codesh::semantic_analyzer::country_symbol &nested_country(
        codesh::semantic_analyzer::country_symbol &parent, const std::string &name, const std::string &bin_fqn)
{
    return parent.get_scope().add_symbol(
        name,
        std::make_unique<codesh::semantic_analyzer::country_symbol>(
            codesh::definition::fully_qualified_name::parse(bin_fqn, codesh::lexer::NO_CODE_POS),
            &parent
        )
    ).first.get();
}

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::lexer::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    country.get_scope().add_symbol(
        codesh::semantic_analyzer::builtins::ALIAS_STRING,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            codesh::definition::fully_qualified_name::parse("java/lang/String", codesh::lexer::NO_CODE_POS),
            nullptr,
            std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>>(),
            std::move(attributes),
            nullptr
        )
    );
}

static void add_alias_labubu(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::lexer::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    country.get_scope().add_symbol(
        codesh::semantic_analyzer::builtins::ALIAS_OBJECT,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            codesh::definition::fully_qualified_name::parse("java/lang/Object", codesh::lexer::NO_CODE_POS),
            nullptr,
            std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>>(),
            std::move(attributes),
            nullptr
        )
    );
}

static void add_alias_het(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::lexer::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    country.get_scope().add_symbol(
        codesh::semantic_analyzer::builtins::ALIAS_EXCEPTION,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            codesh::definition::fully_qualified_name::parse("java/lang/Exception", codesh::lexer::NO_CODE_POS),
            nullptr,
            std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>>(),
            std::move(attributes),
            nullptr
        )
    );
}
