#pragma once

#include "output/ir/instruction.h"

namespace codesh::ast::op
{

class i_math_operator_holder
{
public:
    virtual ~i_math_operator_holder() = default;

    [[nodiscard]] virtual output::ir::operator_type get_ir_operator_type() const = 0;
};

}
