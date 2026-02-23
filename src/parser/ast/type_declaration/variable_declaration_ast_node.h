#pragma once

#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/type/type_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"

namespace codesh::ast::type_decl
{

class variable_declaration_ast_node : public impl::ast_node, public impl::i_constant_pool_emitter
{
    std::string name;
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<attributes_ast_node> attributes;

protected:
    explicit variable_declaration_ast_node(blasphemy::code_position pos);


public:
    [[nodiscard]] const std::string& get_name() const;
    void set_name(const std::string& name);

    [[nodiscard]] type::type_ast_node* get_type() const;
    void set_type(std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] attributes_ast_node* get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);

    void emit_constants(const compilation_unit_ast_node &root_node,
            output::jvm_target::constant_pool &constant_pool) override;
};

}
