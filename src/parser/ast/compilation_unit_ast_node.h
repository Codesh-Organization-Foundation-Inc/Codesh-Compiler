#pragma once

#include "impl/ast_node.h"
#include "import_declaration_ast_node.h"
#include "type_declaration_ast_node.h"

#include <list>
#include <memory>
#include <string>

namespace codesh::ast
{

class compilation_unit_ast_node final : public impl::ast_node
{
    const std::string package_name;
    std::list<std::unique_ptr<import_declaration_ast_node>> import_declarations;
    std::list<std::unique_ptr<type_declaration_ast_node>> type_declarations;

public:
    explicit compilation_unit_ast_node(std::string package_name);

    [[nodiscard]] node_type get_type() const override;

    [[nodiscard]] std::string get_package_name() const;
    [[nodiscard]] const std::list<std::unique_ptr<import_declaration_ast_node>> &get_import_declarations() const;
    [[nodiscard]] const std::list<std::unique_ptr<type_declaration_ast_node>> &get_type_declarations() const;
};

}
