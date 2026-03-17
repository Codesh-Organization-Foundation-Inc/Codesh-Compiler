#include "compilation_unit_ast_node.h"

#include "lexer/source_keyword_info.h"

#include <utility>

codesh::ast::compilation_unit_ast_node::compilation_unit_ast_node(const size_t file_id,
        const definition::basad_type basad_type) :
    ast_node(blasphemy::NO_CODE_POS),
    file_id(file_id),
    source_path(lexer::get_global_source_info_map().at(file_id).path),
    basad_type(basad_type),
    package_name(blasphemy::NO_CODE_POS)
{
}

const std::filesystem::path &codesh::ast::compilation_unit_ast_node::get_source_path() const
{
    return this->source_path;
}

std::string codesh::ast::compilation_unit_ast_node::get_source_stem() const
{
    return source_path.stem();
}

codesh::definition::basad_type codesh::ast::compilation_unit_ast_node::get_basad_type() const
{
    return this->basad_type;
}

codesh::definition::fully_qualified_name &codesh::ast::compilation_unit_ast_node::get_package_name()
{
    return this->package_name;
}

std::vector<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations()
{
    return this->import_declarations;
}

std::vector<std::unique_ptr<codesh::ast::type_decl::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations()
{
    return this->type_declarations;
}

const codesh::definition::fully_qualified_name &codesh::ast::compilation_unit_ast_node::get_package_name() const
{
    return this->package_name;
}

const std::vector<std::unique_ptr<codesh::ast::import_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_import_declarations() const
{
    return this->import_declarations;
}

const std::vector<std::unique_ptr<codesh::ast::type_decl::type_declaration_ast_node>> &codesh::ast::compilation_unit_ast_node::
    get_type_declarations() const
{
    return this->type_declarations;
}
