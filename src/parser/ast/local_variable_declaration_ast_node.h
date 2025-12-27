#pragma once

#include "impl/i_constant_pool_emitter.h"
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

class local_variable_declaration_ast_node : public impl::i_constant_pool_emitter
{
    std::string name;
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<var_reference::value_ast_node> value;
    
    std::unique_ptr<type_decl::attributes_ast_node> attributes;

    int accessible_up_to;

public:
    local_variable_declaration_ast_node();

    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);

    [[nodiscard]] type::type_ast_node *get_type() const;
    void set_type(std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] var_reference::value_ast_node *get_value() const;
    void set_value(std::unique_ptr<var_reference::value_ast_node> value);

    [[nodiscard]] type_decl::attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<type_decl::attributes_ast_node> value);

    [[nodiscard]] int get_accessible_up_to() const;
    void set_accessible_up_to(int available_to);

    void add_to_scope(semantic_analyzer::method_scope_symbol &scope);

    void emit_constants(const compilation_unit_ast_node &root_node,
                output::jvm_target::constant_pool &constant_pool) override;
};

}
