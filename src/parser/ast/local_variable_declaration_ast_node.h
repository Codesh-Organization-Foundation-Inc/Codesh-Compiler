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

    bool final;

public:
    local_variable_declaration_ast_node();

    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);
    [[nodiscard]] type::type_ast_node *get_type() const;
    void set_type(std::unique_ptr<type::type_ast_node> type);
    [[nodiscard]] bool is_final() const;
    void set_is_final(bool is_final);
};

}
