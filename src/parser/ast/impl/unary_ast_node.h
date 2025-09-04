#pragma once

#include "typed_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

template <typename T>
class unary_ast_node : public typed_ast_node<T>
{
    std::unique_ptr<typed_ast_node<T>> child;

protected:
    explicit unary_ast_node(std::unique_ptr<typed_ast_node<T>> child);

public:
    [[nodiscard]] typed_ast_node<T> *get_child() const;
};

}
