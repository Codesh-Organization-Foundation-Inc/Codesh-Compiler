#pragma once

#include "attributes_ast_node.h"
#include "defenition/fully_qualified_name.h"
#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_descriptor_emitter.h"

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "field_declaration_ast_node.h"
#include "output/jvm_target/constant_pool.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"
#include "parser/ast/impl/i_resolvable.h"

namespace codesh::semantic_analyzer
{
class type_symbol;
}
namespace codesh::ast::method
{
class method_declaration_ast_node;
class constructor_declaration_ast_node;
}
namespace codesh::ast::type
{
class custom_type_ast_node;
}


namespace codesh::ast::type_decl
{

class type_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter,
    public impl::i_resolvable<semantic_analyzer::type_symbol>, public impl::i_constant_pool_emitter
{
    std::unique_ptr<output::jvm_target::constant_pool> constant_pool;

    std::unique_ptr<type::custom_type_ast_node> super_class;
    std::vector<std::unique_ptr<type::custom_type_ast_node>> interfaces;

    const definition::fully_qualified_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> resolved_symbol;

    std::vector<std::unique_ptr<field_declaration_ast_node>> fields;

    std::unique_ptr<attributes_ast_node> attributes;
    

    std::deque<std::unique_ptr<method::method_declaration_ast_node>> all_methods;
    std::vector<method::method_declaration_ast_node *> methods;
    std::vector<method::constructor_declaration_ast_node *> constructors;

    static void emit_metadata(const compilation_unit_ast_node &root_node,
            output::jvm_target::constant_pool &constant_pool);

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> &_get_resolved()
        const override;

public:
    type_declaration_ast_node(blasphemy::code_position code_position, definition::fully_qualified_name name);
    ~type_declaration_ast_node() override;

    void set_resolved(semantic_analyzer::type_symbol &symbol) override;

    [[nodiscard]] const output::jvm_target::constant_pool &get_constant_pool() const;
    void set_constant_pool(output::jvm_target::constant_pool constant_pool);


    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] const definition::fully_qualified_name &get_unresolved_name() const override;


    [[nodiscard]] type::custom_type_ast_node *get_super_class() const;
    void set_super_class(std::unique_ptr<type::custom_type_ast_node> super_class);

    [[nodiscard]] const std::vector<std::unique_ptr<field_declaration_ast_node>>& get_fields() const;
    void add_field(std::unique_ptr<field_declaration_ast_node> field);

    [[nodiscard]] attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);

    /**
     * @return All methods, including constructors.
     * Constructors are placed first, then methods.
     */
    [[nodiscard]] const std::deque<std::unique_ptr<method::method_declaration_ast_node>> &get_all_methods() const;
    void add_method(std::unique_ptr<method::method_declaration_ast_node> method_decl);
    void add_constructor(std::unique_ptr<method::constructor_declaration_ast_node> constructor_decl);

    [[nodiscard]] const std::vector<method::constructor_declaration_ast_node *> &get_constructors() const;
    [[nodiscard]] const std::vector<method::method_declaration_ast_node *> &get_methods() const;

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;
};

}
