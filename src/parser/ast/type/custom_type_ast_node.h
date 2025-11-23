#pragma once

#include "../impl/i_resolvable.h"
#include "type_ast_node.h"

#include <optional>
#include <string>

namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class custom_type_ast_node : public type_ast_node, public impl::i_resolvable
{
    const std::string name;
    std::optional<std::string> resolved_name;

protected:
    [[nodiscard]] std::optional<std::string> &get_resolved_name() override;

public:
    explicit custom_type_ast_node(std::string name);

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] std::string get_name() const override;
    [[nodiscard]] const std::optional<std::string> &get_resolved_name() const override;

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;
};

}
