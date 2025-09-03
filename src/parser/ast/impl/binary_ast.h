#pragma once
#include "ast_node.h"

#include <memory>

namespace codesh::ast::impl
{


class binary_ast : public ast_node
{
    std::unique_ptr<ast_node> left, right;

public:
    [[nodiscard]] ast_node &get_left() const;
    [[nodiscard]] ast_node &get_right() const;
};


}
