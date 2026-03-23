#pragma once

#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_descriptor_emitter.h"
#include "parser/ast/impl/i_resolvable.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"

#include "parser/ast/type/custom_type_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"

#include "output/jvm_target/constant_pool.h"

#include <deque>
#include <memory>
#include <optional>
#include <vector>

namespace codesh::semantic_analyzer
{
class type_symbol;
}

namespace codesh::ast::type_decl
{

class interface_declaration_ast_node final : public impl::ast_node, public impl::i_descriptor_emitter,
      public impl::i_resolvable<semantic_analyzer::type_symbol>, public impl::i_constant_pool_emitter
{
    std::unique_ptr<output::jvm_target::constant_pool> constant_pool;

    const definition::fully_qualified_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> resolved_symbol;

    std::vector<std::unique_ptr<type::custom_type_ast_node>> extended_interfaces;

    std::deque<std::unique_ptr<method::method_declaration_ast_node>> methods;

    std::unique_ptr<attributes_ast_node> attributes;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> &_get_resolved()
        const override;

public:
    interface_declaration_ast_node(lexer::code_position code_position, definition::fully_qualified_name name);

    ~interface_declaration_ast_node() override;

    void set_resolved(semantic_analyzer::type_symbol &symbol) override;

    [[nodiscard]] const definition::fully_qualified_name &get_unresolved_name() const override;

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    void add_extended_interface(std::unique_ptr<type::custom_type_ast_node> interface);

    [[nodiscard]] const std::vector<std::unique_ptr<type::custom_type_ast_node>> &
    get_extended_interfaces() const;

    void add_method(std::unique_ptr<method::method_declaration_ast_node> method);

    [[nodiscard]] const std::deque<std::unique_ptr<method::method_declaration_ast_node>> &
    get_methods() const;

    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);

    [[nodiscard]] attributes_ast_node *get_attributes() const;

    [[nodiscard]] const output::jvm_target::constant_pool &get_constant_pool() const;

    void set_constant_pool(output::jvm_target::constant_pool constant_pool);

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;
};

}