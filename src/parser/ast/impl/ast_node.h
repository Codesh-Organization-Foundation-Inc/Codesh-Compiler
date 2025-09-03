#pragma once

namespace codesh::ast
{
enum class node_type;
}
namespace codesh
{
enum class token_group;
}


namespace codesh::ast::impl
{


class ast_node
{
public:
    virtual ~ast_node();

  /**
   * @return The token group associated with this node.
   * If this is `token_group::count`
   */
    [[nodiscard]] virtual node_type get_type() const = 0;
};


}