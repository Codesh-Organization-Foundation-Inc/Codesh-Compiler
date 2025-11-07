#pragma once

#include "impl/ast_node.h"
#include "type/type_ast_node.h"

#include <memory>
#include <string>

namespace codesh::ast
{

class local_variable_declaration_ast_node : public impl::ast_node
{
    std::string name;
    std::unique_ptr<type::type_ast_node> type;

    bool is_final;

public:
    local_variable_declaration_ast_node();
};

}
