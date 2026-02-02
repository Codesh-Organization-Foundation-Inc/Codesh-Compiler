#pragma once

#include "blasphemy/blasphemy_collector.h"

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
    const blasphemy::code_position code_position;

public:
    explicit ast_node(blasphemy::code_position code_position);
    virtual ~ast_node();

    [[nodiscard]] blasphemy::code_position get_code_position() const;
};

}