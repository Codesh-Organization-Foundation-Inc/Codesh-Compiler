#pragma once
#include "typed_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

template <typename T>
class binary_ast_node : public typed_ast_node<T>
{
    std::unique_ptr<typed_ast_node<T>> left, right;

public:
    [[nodiscard]] typed_ast_node<T> &get_left() const;
    [[nodiscard]] typed_ast_node<T> &get_right() const;
};


}

#include "binary_ast_node.tpp"
