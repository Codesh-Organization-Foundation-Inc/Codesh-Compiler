#pragma once

#include "type_ast_node.h"

namespace codesh::ast::type
{

class null_type_ast_node final : public type_ast_node
{
protected:
    [[nodiscard]] output::ir::instruction_type _to_instruction_type() const override;
    [[nodiscard]] type_ast_node *_clone() const override;

public:
    explicit null_type_ast_node(lexer::code_position code_position);

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] std::string to_pretty_string() const override;
};

}
