#pragma once

#pragma once

#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/type/type_ast_node.h"
#include "parser/ast/type_declaration/attributes_ast_node.h"

#include <memory>
#include <string>

namespace codesh::ast //FIXME: change the namespace to the correct one if i need to
{

class variable_declaration_ast_node : public impl::ast_node, public impl::i_constant_pool_emitter
{

protected:
    std::string name;
    std::unique_ptr<type::type_ast_node> type;
    std::unique_ptr<type_decl::attributes_ast_node> attributes;

    variable_declaration_ast_node(blasphemy::code_position pos, std::string name,
            std::unique_ptr<type::type_ast_node> type);

    void emit_common_constants(output::jvm_target::constant_pool& constant_pool) const;

public:
    virtual ~variable_declaration_ast_node() = default;

    [[nodiscard]] const std::string& get_name() const;
    void set_name(const std::string& name);

    [[nodiscard]] type::type_ast_node* get_type() const;
    void set_type(std::unique_ptr<type::type_ast_node> type);

    [[nodiscard]] type_decl::attributes_ast_node* get_attributes() const;
    void set_attributes(std::unique_ptr<type_decl::attributes_ast_node> attributes);
};

}