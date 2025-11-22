#pragma once

#include "../impl/ast_node.h"
#include "../impl/i_descriptor_emitter.h"
#include "attributes_ast_node.h"

#include <memory>
#include <string>

#include "../../../output/jvm_target/constant_pool.h"
#include "../impl/i_resolvable.h"

namespace codesh::ast::type_decl
{

class type_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter, public impl::i_resolvable
{
    std::unique_ptr<output::jvm_target::constant_pool> constant_pool;

    const std::string name;
    std::optional<std::string> resolved_name;

    std::unique_ptr<attributes_ast_node> attributes;

public:
    explicit type_declaration_ast_node(std::string name);

    [[nodiscard]] std::string generate_descriptor() const override;
    /**
     * @return The class name prefixed by the package, separated by slashes
     */
    [[nodiscard]] std::string get_binary_name() const override;
    void set_resolved_name(std::string resolved_name) override;


    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);

    [[nodiscard]] std::optional<std::reference_wrapper<const output::jvm_target::constant_pool>> get_constant_pool() const;
    void set_constant_pool(output::jvm_target::constant_pool constant_pool);
};

}
