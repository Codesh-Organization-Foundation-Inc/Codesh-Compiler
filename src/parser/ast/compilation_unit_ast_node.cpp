#include "compilation_unit_ast_node.h"

#include <utility>

codesh::ast::compilation_unit_ast_node::compilation_unit_ast_node(const definition::basad_type basad_type,
        std::string source_stem) :
    source_stem(std::move(source_stem)),
    basad_type(basad_type)
{
}

std::string codesh::ast::compilation_unit_ast_node::get_source_stem() const
{
    return this->source_stem;
}

codesh::definition::basad_type codesh::ast::compilation_unit_ast_node::get_basad_type() const
{
    return this->basad_type;
}

codesh::definition::fully_qualified_class_name &codesh::ast::compilation_unit_ast_node::get_package_name()
{
    return this->package_name;
}

std::list<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations()
{
    return this->import_declarations;
}

std::list<std::unique_ptr<codesh::ast::type_decl::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations()
{
    return this->type_declarations;
}

const codesh::definition::fully_qualified_class_name &codesh::ast::compilation_unit_ast_node::get_package_name() const
{
    return this->package_name;
}

const std::list<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations() const
{
    return this->import_declarations;
}

const std::list<std::unique_ptr<codesh::ast::type_decl::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations() const
{
    return this->type_declarations;
}

std::optional<std::reference_wrapper<const codesh::semantic_analyzer::symbol_table>> codesh::ast::
    compilation_unit_ast_node::get_symbol_table() const
{
    if (symbol_table)
        return symbol_table.value();

    return std::nullopt;
}

std::optional<std::reference_wrapper<codesh::semantic_analyzer::symbol_table>> codesh::ast::compilation_unit_ast_node::
    get_symbol_table()
{
    if (symbol_table)
        return symbol_table.value();

    return std::nullopt;
}

void codesh::ast::compilation_unit_ast_node::construct_symbol_table()
{
    symbol_table.emplace(*this);
}
