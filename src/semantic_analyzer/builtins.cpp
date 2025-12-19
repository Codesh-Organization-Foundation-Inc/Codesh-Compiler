#include "builtins.h"

#include "../parser/ast/type/primitive_type_ast_node.h"
#include "symbol_table/symbol.h"
#include "symbol_table/symbol_table.h"

static void add_class_massof(codesh::semantic_analyzer::country_symbol &country);
static void add_method_emor(codesh::semantic_analyzer::type_symbol &massof_symbol);

static constexpr std::string CLASS_MASSOF = "מסוף";
static constexpr std::string METHOD_EMOR = "אמר";


void codesh::semantic_analyzer::builtins::add_builtins(const symbol_table &table)
{
    country_symbol &country = table.resolve_country("").value();

    add_class_massof(country);
}

static void add_class_massof(codesh::semantic_analyzer::country_symbol &country)
{
    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>();
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_final(true);

    auto &massof_symbol = country.add_symbol(
        CLASS_MASSOF,
        std::make_unique<codesh::semantic_analyzer::type_symbol>(
            &country,
            country.get_full_name().with(CLASS_MASSOF),

            std::move(attributes),

            nullptr
        )
    ).first.get();


    // Functions
    add_method_emor(massof_symbol);
}

static void add_method_emor(codesh::semantic_analyzer::type_symbol &massof_symbol)
{
    auto &emor_overloads = massof_symbol.add_symbol(
        METHOD_EMOR,
        std::make_unique<codesh::semantic_analyzer::method_overloads_symbol>(
            &massof_symbol
        )
    ).first.get();


    auto attributes = std::make_unique<codesh::ast::type_decl::attributes_ast_node>();
    attributes->set_visibility(codesh::definition::visibility::PUBLIC);
    attributes->set_is_static(true);

    std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> parameter_types;
    parameter_types.reserve(1);
    parameter_types.push_back(std::make_unique<codesh::ast::type::custom_type_ast_node>("Ljava/lang/String;"));

    auto return_type = std::make_unique<codesh::ast::type::primitive_type_ast_node>(codesh::definition::primitive_type::VOID);

    // Make the method symbol point to the original PrintStream's println
    auto &emor_symbol = emor_overloads.add_symbol(
        "(Ljava/lang/String;)V",
        std::make_unique<codesh::semantic_analyzer::method_symbol>(
            &emor_overloads,
            massof_symbol,
            "java/io/PrintStream/println",

            std::move(attributes),
            std::move(parameter_types),
            std::move(return_type),

            nullptr
        )
    ).first.get();
}
