#pragma once

#include "../../../defenition/primitive_type.h"
#include "type_ast_node.h"

namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class primitive_type_ast_node : public type_ast_node
{
    const definition::primitive_type type;

public:
    explicit primitive_type_ast_node(definition::primitive_type type);

    [[nodiscard]] std::string generate_descriptor() const override;

    [[nodiscard]] definition::primitive_type get_type() const;

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;
};

}
