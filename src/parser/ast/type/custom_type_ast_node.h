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
    std::string name;
    std::optional<std::string> resolved_name;

public:
    [[nodiscard]] std::string generate_descriptor() const override;

    [[nodiscard]] std::string get_binary_name() const override;
    void set_resolved_name(std::string resolved_name) override;

    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;
};

}
