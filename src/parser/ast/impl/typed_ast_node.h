#pragma once

#include "ast_node.h"

namespace codesh::ast::impl
{

template <typename T>
class typed_ast_node : public ast_node
{
    //TODO: Change this to "compile": AST to IR.
    // Then remove the template and all subsequent .tpp's to cpp's.
    [[nodiscard]] virtual T evaluate() const = 0;
};

}

