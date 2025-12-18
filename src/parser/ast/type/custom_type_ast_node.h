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
    const definition::fully_qualified_class_name name;
    std::optional<definition::fully_qualified_class_name> resolved_name;

protected:
    [[nodiscard]] std::optional<definition::fully_qualified_class_name> &_get_resolved_name() override;
    [[nodiscard]] const std::optional<definition::fully_qualified_class_name> &_get_resolved_name() const override;

public:
    explicit custom_type_ast_node(definition::fully_qualified_class_name name);

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    using i_resolvable::get_name;
    [[nodiscard]] const definition::fully_qualified_class_name &get_name() const override;

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;
};

}
