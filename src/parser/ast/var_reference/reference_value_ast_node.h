#pragma once

#include "../../../defenition/fully_qualified_class_name.h"
#include "value_ast_node.h"

namespace codesh::semantic_analyzer
{
class type_symbol;
}

//TODO: Make resolveable
class reference_value_ast_node : public codesh::ast::var_reference::value_ast_node
{
    const codesh::definition::fully_qualified_class_name name;

public:
    explicit reference_value_ast_node(codesh::definition::fully_qualified_class_name name);

    [[nodiscard]] const codesh::definition::fully_qualified_class_name &get_unresolved_name() const;
};