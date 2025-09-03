#pragma once
#include "typed_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

template <typename T>
class binary_ast_node : public typed_ast_node<T>
{
    std::unique_ptr<typed_ast_node<T>> left, right;

protected:
    binary_ast_node(std::unique_ptr<typed_ast_node<T>> left, std::unique_ptr<typed_ast_node<T>> right);
public:
    [[nodiscard]] typed_ast_node<T> &get_left() const;
    [[nodiscard]] typed_ast_node<T> &get_right() const;

    void set_left(typed_ast_node<T>& node);
    void set_right(typed_ast_node<T>& node);



};


}

#include "binary_ast_node.tpp"
