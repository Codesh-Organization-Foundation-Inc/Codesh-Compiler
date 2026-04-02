#include "builtins.h"

#include "defenition/fully_qualified_name.h"
#include "lexer/source_file_info.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "semantic_analyzer/util.h"

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country);
static void add_alias_labubu(codesh::semantic_analyzer::country_symbol &country);
static void add_alias_het(codesh::semantic_analyzer::country_symbol &country);

static void resolve_builtin_alias(codesh::semantic_analyzer::country_symbol &country,
        const codesh::semantic_analyzer::symbol_table &table,
        const std::string &alias_name, const std::string &bin_fqn);


void codesh::semantic_analyzer::builtins::collect_builtins(country_symbol &country)
{
    add_alias_ktuvim(country);
    add_alias_labubu(country);
    add_alias_het(country);
}

void codesh::semantic_analyzer::builtins::resolve_builtins(country_symbol &country,
        const symbol_table &table)
{
    resolve_builtin_alias(country, table, ALIAS_STRING, "java/lang/String");
    resolve_builtin_alias(country, table, ALIAS_OBJECT, "java/lang/Object");
    resolve_builtin_alias(country, table, ALIAS_EXCEPTION, "java/lang/Exception");
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

static void resolve_builtin_alias(codesh::semantic_analyzer::country_symbol &country,
        const codesh::semantic_analyzer::symbol_table &table, const std::string &alias_name, const std::string &bin_fqn)
{
    const auto fqn = codesh::definition::fully_qualified_name::parse(
        bin_fqn,
        codesh::lexer::NO_CODE_POS
    );

    if (!table.try_load_candidate(fqn))
        return;

    codesh::semantic_analyzer::country_symbol &java_country = codesh::semantic_analyzer::util::find_or_create_country(
        table,
        fqn.omit_last()
    );

    //NOTE: The process below will REMOVE the actual interop'd symbol and MOVE it to the provided country.
    // Though this is okie as it gets re-loaded later if needed :3
    auto loaded = java_country.get_scope().resolve_and_move(fqn.get_last_part());
    if (loaded == nullptr)
        return;

    country.get_scope().remove_symbol(alias_name);
    country.get_scope().add_symbol(
        alias_name,
        std::unique_ptr<codesh::semantic_analyzer::type_symbol>(
            static_cast<codesh::semantic_analyzer::type_symbol *>(loaded.release()) // NOLINT(*-pro-type-static-cast-downcast)
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
            codesh::definition::fully_qualified_name::parse(
                "java/lang/Exception",
                codesh::lexer::NO_CODE_POS
            ),
            nullptr,
            std::vector<std::unique_ptr<codesh::ast::type::custom_type_ast_node>>(),
            std::move(attributes),
            nullptr
        )
    );
}
