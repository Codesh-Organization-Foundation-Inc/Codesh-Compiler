#pragma once

#include "impl/ast_node.h"
#include "impl/typed_ast_node.h"

#include <memory>

namespace codesh::ast
{
class block_ast_node;
}
namespace codesh::ast
{


class while_ast_node final : public impl::ast_node
{
    std::unique_ptr<typed_ast_node<bool>> condition;
    std::unique_ptr<block_ast_node> block;

public:
    while_ast_node(std::unique_ptr<typed_ast_node<bool>> condition, std::unique_ptr<block_ast_node> block);

    [[nodiscard]] node_type get_type() const override;
};

}