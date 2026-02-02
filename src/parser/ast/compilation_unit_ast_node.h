#pragma once

#include "defenition/basad_type.h"
#include "parser/ast/impl/ast_node.h"
#include "import_declaration_ast_node.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"

#include <memory>
#include <vector>
#include <filesystem>
#include <string>

namespace codesh::ast
{

class compilation_unit_ast_node final : public impl::ast_node
{
    const std::filesystem::path source_path;

    const definition::basad_type basad_type;
    definition::fully_qualified_name package_name;
    std::vector<std::unique_ptr<import_declaration_ast_node>> import_declarations;
    std::vector<std::unique_ptr<type_decl::type_declaration_ast_node>> type_declarations;

public:
    compilation_unit_ast_node(definition::basad_type basad_type, std::filesystem::path source_path);

    [[nodiscard]] const std::filesystem::path &get_source_path() const;
    /**
     * @returns The source file name with no extension
     */
    [[nodiscard]] std::string get_source_stem() const;

    [[nodiscard]] definition::basad_type get_basad_type() const;
    [[nodiscard]] definition::fully_qualified_name &get_package_name();
    [[nodiscard]] std::vector<std::unique_ptr<import_declaration_ast_node>> &get_import_declarations();
    [[nodiscard]] std::vector<std::unique_ptr<type_decl::type_declaration_ast_node>> &get_type_declarations();


    [[nodiscard]] const definition::fully_qualified_name &get_package_name() const;
    [[nodiscard]] const std::vector<std::unique_ptr<import_declaration_ast_node>> &get_import_declarations() const;
    [[nodiscard]] const std::vector<std::unique_ptr<type_decl::type_declaration_ast_node>> &get_type_declarations() const;
};

}
