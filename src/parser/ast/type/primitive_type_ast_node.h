#pragma once

#include "defenition/primitive_type.h"
#include "type_ast_node.h"

namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class primitive_type_ast_node final : public type_ast_node
{
    const definition::primitive_type type;

protected:
    [[nodiscard]] output::ir::instruction_type _to_instruction_type() const override;
    [[nodiscard]] type_ast_node *_clone() const override;

public:
    primitive_type_ast_node(lexer::code_position code_position, definition::primitive_type type);

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] definition::primitive_type get_type() const;

    // ReSharper disable once CppHidingFunction
    [[nodiscard]] std::unique_ptr<primitive_type_ast_node> clone() const;

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
