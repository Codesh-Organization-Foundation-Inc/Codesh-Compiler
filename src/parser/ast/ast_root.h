#pragma once

#include "impl/multichild_ast.h"


namespace codesh::ast
{


class ast_root final : public impl::multichild_ast
{
public:
    [[nodiscard]] node_type get_type() const override;
};


}
