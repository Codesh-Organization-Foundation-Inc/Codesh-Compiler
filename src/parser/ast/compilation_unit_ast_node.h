#pragma once

#include "../../defenition/basad_type.h"
#include "impl/ast_node.h"
#include "import_declaration_ast_node.h"
#include "type_declaration/type_declaration_ast_node.h"

#include <list>
#include <memory>
#include <string>

namespace codesh::ast
{

class compilation_unit_ast_node final : public impl::ast_node
{
    const std::string source_stem;

    const definition::basad_type basad_type;
    std::list<std::string> package_name;
    std::list<std::unique_ptr<import_declaration_ast_node>> import_declarations;
    std::list<std::unique_ptr<type_decl::type_declaration_ast_node>> type_declarations;

public:
    explicit compilation_unit_ast_node(definition::basad_type basad_type, std::string source_stem);

    /**
     * The source file name with no extension
     */
    [[nodiscard]] std::string get_source_stem() const;

    [[nodiscard]] definition::basad_type get_basad_type() const;
    [[nodiscard]] std::list<std::string> &get_package_name();
    [[nodiscard]] std::list<std::unique_ptr<import_declaration_ast_node>> &get_import_declarations();
    [[nodiscard]] std::list<std::unique_ptr<type_decl::type_declaration_ast_node>> &get_type_declarations();


    [[nodiscard]] const std::list<std::string> &get_package_name() const;
    [[nodiscard]] const std::list<std::unique_ptr<import_declaration_ast_node>> &get_import_declarations() const;
    [[nodiscard]] const std::list<std::unique_ptr<type_decl::type_declaration_ast_node>> &get_type_declarations() const;
};

}
