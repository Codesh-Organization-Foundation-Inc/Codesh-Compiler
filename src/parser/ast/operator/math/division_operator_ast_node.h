#pragma once

#include "../../impl/binary_ast_node.h"

namespace codesh::ast::op
{

class division_operator_ast_node final : public impl::binary_ast_node
{
public:
    division_operator_ast_node(std::unique_ptr<ir_convertable_ast_node> left, std::unique_ptr<ir_convertable_ast_node> right);

    void emit_ir(output::ir::code_block &containing_block) const override;
};

}

