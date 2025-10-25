#pragma once

#include "impl/ast_node.h"

#include <list>
#include <memory>

namespace codesh::ast
{

class block_ast_node final : public impl::ast_node
{
    std::list<std::unique_ptr<ast_node>> children;

public:
    [[nodiscard]] std::list<std::unique_ptr<ast_node>> &get_children();
};

}
