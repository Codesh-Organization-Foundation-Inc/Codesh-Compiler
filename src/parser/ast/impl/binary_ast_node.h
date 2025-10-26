#pragma once
#include "method_ast_node.h"

#include <memory>

namespace codesh::ast::impl
{

class binary_ast_node : public method_ast_node
{
    std::unique_ptr<method_ast_node> left, right;

protected:
    binary_ast_node(std::unique_ptr<method_ast_node> left, std::unique_ptr<method_ast_node> right);

public:
    [[nodiscard]] method_ast_node *get_left() const;
    [[nodiscard]] method_ast_node *get_right() const;

    void set_left(std::unique_ptr<method_ast_node> node);
    void set_right(std::unique_ptr<method_ast_node> node);
};


}

