#pragma once

#include "impl/ast_node.h"
#include "type/type_ast_node.h"
#include "type_declaration/attributes_ast_node.h"
#include "var_reference/value_ast_node.h"

#include <memory>
#include <string>

namespace codesh::semantic_analyzer
{
class method_scope_symbol;
}
namespace codesh::ast
{

class variable_declaration_ast_node : public impl::ast_node
{
    std::string name;
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<var_reference::value_ast_node> value;
    
    std::unique_ptr<type_decl::attributes_ast_node> attributes;

public:
    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);

    [[nodiscard]] type::type_ast_node *get_type() const;
    void set_type(std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] var_reference::value_ast_node *get_value() const;
    void set_value(std::unique_ptr<var_reference::value_ast_node> value);

    [[nodiscard]] type_decl::attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<type_decl::attributes_ast_node> value);


    void add_to_scope(semantic_analyzer::method_scope_symbol &scope);
};

}
