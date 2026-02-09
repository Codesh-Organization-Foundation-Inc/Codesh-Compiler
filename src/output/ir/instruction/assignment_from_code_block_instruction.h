#pragma once

#include "assignment_instruction.h"
#include "output/ir/code_block.h"

namespace codesh::ast::var_reference
{
class value_ast_node;
}

namespace codesh::output::ir
{

/**
 * An assignment where the RHS is determined by the provided code block
 */
class assignment_from_code_block_instruction final : public assignment_instruction
{
    const code_block _code_block;

protected:
    void emit_rhs(std::vector<instruction_container> &collector) const override;
    [[nodiscard]] size_t rhs_size() const override;

public:
    assignment_from_code_block_instruction(instruction_type type, operator_type op_type, int target_lvt_index,
            code_block _code_block);
};

}
