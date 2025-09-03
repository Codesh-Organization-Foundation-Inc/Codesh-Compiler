#pragma once

#include "impl/ast_node.h"

#include <list>
#include <memory>

namespace codesh::ast
{

class block_ast_node final : public impl::ast_node
{
public:
    std::list<std::unique_ptr<ast_node>> children;

    [[nodiscard]] node_type get_type() const override;
};

}
