#pragma once

#include "ast_node.h"

namespace codesh::ast::impl
{

template <typename T>
class typed_ast_node : public ast_node
{
    [[nodiscard]] virtual T evaluate() const = 0;
};

}

