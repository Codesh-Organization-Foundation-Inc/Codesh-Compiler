#pragma once

#include "../impl/ast_node.h"
#include "../impl/i_descriptor_emitter.h"
#include "attributes_ast_node.h"

#include <memory>
#include <string>

#include "../../../output/jvm_target/constant_pool.h"
#include "../impl/i_resolvable.h"
#include "../type/custom_type_ast_node.h"
#include "../method/constructor_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

class type_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter, public impl::i_resolvable
{
    std::unique_ptr<output::jvm_target::constant_pool> constant_pool;

    std::unique_ptr<type::custom_type_ast_node> super_class;
    //TODO: Add implements

    const std::string name;
    std::optional<std::string> resolved_name;

    std::unique_ptr<attributes_ast_node> attributes;


    std::list<std::unique_ptr<method::method_declaration_ast_node>> methods;

protected:
    [[nodiscard]] std::optional<std::string> &get_resolved_name() override;

public:
    explicit type_declaration_ast_node(std::string name);

    [[nodiscard]] std::optional<std::reference_wrapper<const output::jvm_target::constant_pool>> get_constant_pool()
        const;
    void set_constant_pool(output::jvm_target::constant_pool constant_pool);


    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] const std::optional<std::string> &get_resolved_name() const override;
    [[nodiscard]] std::string get_name() const override;


    [[nodiscard]] type::custom_type_ast_node *get_super_class() const;
    void set_super_class(std::unique_ptr<type::custom_type_ast_node> super_class);


    [[nodiscard]] attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);


    [[nodiscard]] std::list<std::unique_ptr<method::method_declaration_ast_node>> &get_methods();
    [[nodiscard]] const std::list<std::unique_ptr<method::method_declaration_ast_node>> &get_methods() const;
};

}
