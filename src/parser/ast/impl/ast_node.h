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
};


}