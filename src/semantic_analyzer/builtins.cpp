#include "builtins.h"

#include "blasphemy/blasphemy_collector.h"
#include "lexer/trie/keywords.h"
#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"
#include "semantic_analyzer/symbol_table/symbol_table.h"
#include "semantic_analyzer/util.h"

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country);
static void add_alias_labubu(codesh::semantic_analyzer::country_symbol &country);
static void add_class_massof(codesh::semantic_analyzer::country_symbol &country);
static void add_method_emor(codesh::semantic_analyzer::type_symbol &massof_symbol);

static void add_emor_symbol(codesh::semantic_analyzer::method_overloads_symbol &emor_overloads,
        codesh::semantic_analyzer::type_symbol &massof_symbol,
        std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> parameter_types,
        std::string method_descriptor);

static void add_method_emor_string(codesh::semantic_analyzer::method_overloads_symbol &emor_overloads,
        codesh::semantic_analyzer::type_symbol &massof_symbol);
static void add_method_emor_int(codesh::semantic_analyzer::method_overloads_symbol &emor_overloads,
        codesh::semantic_analyzer::type_symbol &massof_symbol);


//TODO: Remove:
static constexpr std::string ALIAS_STD_OUT = "פלט";

static constexpr std::string ALIAS_LABUBU = "לאבובו";

static constexpr std::string CLASS_MASSOF = "מסוף";
static constexpr std::string METHOD_EMOR = "אמר";


void codesh::semantic_analyzer::builtins::add_builtins(const symbol_table &table)
{
    //TODO: Properly wrap in countries
    country_symbol &country = table.resolve_country("").value();

    add_alias_ktuvim(country);
    add_alias_labubu(country);
    add_class_massof(country);
}

static void add_alias_ktuvim(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    country.get_scope().add_symbol(
        codesh::lexer::trie::keyword::ALIAS_STRING,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            "java/lang/String",

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
        ALIAS_LABUBU,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            "java/lang/Object",

            std::move(attributes),

            nullptr
        )
    );
}

static void add_class_massof(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    auto &massof_symbol = codesh::semantic_analyzer::util::add_type_symbol(
        country,
        CLASS_MASSOF,
        std::move(attributes)
    ).first.get();


    // Add System.out
    auto is_attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);
    is_attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    is_attributes->set_is_final(true);
    is_attributes->set_is_static(true);

    massof_symbol.get_scope().add_symbol(
        ALIAS_STD_OUT,
        std::make_unique<codesh::semantic_analyzer::field_symbol>(
            &massof_symbol,
            "java/lang/System/out",

            std::move(is_attributes),
            std::make_unique<codesh::ast::type::custom_type_ast_node>(
                codesh::blasphemy::NO_CODE_POS,
                "java/io/PrintStream"
            )
        )
    );


    // Functions
    add_method_emor(massof_symbol);
}

static void add_method_emor(codesh::semantic_analyzer::type_symbol &massof_symbol)
{
    auto &emor_overloads = massof_symbol.get_scope().add_symbol(
        METHOD_EMOR,
        std::make_unique<codesh::semantic_analyzer::method_overloads_symbol>(
            &massof_symbol
        )
    ).first.get();

    add_method_emor_string(emor_overloads, massof_symbol);
    add_method_emor_int(emor_overloads, massof_symbol);
}

static void add_method_emor_string(codesh::semantic_analyzer::method_overloads_symbol &emor_overloads,
        codesh::semantic_analyzer::type_symbol &massof_symbol)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> parameter_types;
    parameter_types.reserve(1);
    parameter_types.push_back(std::make_unique<codesh::ast::type::custom_type_ast_node>(
        codesh::blasphemy::NO_CODE_POS,
        "java/lang/String"
    ));

    add_emor_symbol(
        emor_overloads,
        massof_symbol,
        std::move(parameter_types),
        "(Ljava/lang/String;)V"
    );
}

static void add_method_emor_int(codesh::semantic_analyzer::method_overloads_symbol &emor_overloads,
        codesh::semantic_analyzer::type_symbol &massof_symbol)
{
    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> parameter_types;
    parameter_types.reserve(1);
    parameter_types.push_back(std::make_unique<codesh::ast::type::primitive_type_ast_node>(
        codesh::blasphemy::NO_CODE_POS,
        codesh::definition::primitive_type::INTEGER
    ));

    add_emor_symbol(
        emor_overloads,
        massof_symbol,
        std::move(parameter_types),
        "(I)V"
    );
}

static void add_emor_symbol(codesh::semantic_analyzer::method_overloads_symbol &emor_overloads,
        codesh::semantic_analyzer::type_symbol &massof_symbol,
        std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> parameter_types,
        std::string method_descriptor)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>(codesh::blasphemy::NO_CODE_POS);
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_static(true);

    auto return_type = std::make_unique<codesh::ast::type::primitive_type_ast_node>(
        codesh::blasphemy::NO_CODE_POS,
        codesh::definition::primitive_type::VOID
    );

    // Make the method symbol point to the original PrintStream's println
    emor_overloads.get_scope().add_symbol(
        std::move(method_descriptor),
        std::make_unique<codesh::semantic_analyzer::method_symbol>(
            &emor_overloads,
            massof_symbol,
            "java/io/PrintStream/println",

            std::move(attributes),
            std::move(parameter_types),
            std::move(return_type),

            nullptr
        )
    );
}
