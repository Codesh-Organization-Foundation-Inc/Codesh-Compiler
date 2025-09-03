#pragma once

#include "ast_node.h"

#include <list>
#include <memory>

namespace codesh::ast::impl
{


class multichild_ast_node : public ast_node
{
public:
    std::list<std::unique_ptr<ast_node>> children;
};


}