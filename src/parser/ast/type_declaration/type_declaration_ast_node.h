#pragma once

#include "../impl/ast_node.h"
#include "../impl/i_descriptor_emitter.h"
#include "attributes_ast_node.h"

#include <memory>
#include <string>

namespace codesh::ast::type_decl
{

class type_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter
{
    const std::string name;
    std::unique_ptr<attributes_ast_node> attributes;

public:
    explicit type_declaration_ast_node(std::string name);

    [[nodiscard]] std::string generate_descriptor() const override;
    /**
     * @return The class name prefixed by the package, separated by slashes
     */
    [[nodiscard]] std::string get_binary_name() const;


    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<attributes_ast_node> attributes);
};

}
