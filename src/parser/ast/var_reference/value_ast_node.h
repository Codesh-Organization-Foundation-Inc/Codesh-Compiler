#pragma once

#include "../impl/ast_node.h"
#include "../type/type_ast_node.h"

namespace codesh::ast::var_reference
{

class value_ast_node : public impl::ast_node
{
    const std::unique_ptr<type::type_ast_node> type;

public:
    explicit value_ast_node(std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] const type::type_ast_node &get_type() const;
};

}
