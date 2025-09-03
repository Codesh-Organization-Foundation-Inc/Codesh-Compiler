#pragma once

#include "ast_node.h"

namespace codesh::ast
{

template <typename T>
class typed_ast_node : public impl::ast_node
{
    [[nodiscard]] virtual T evaluate() const = 0;
};

}

