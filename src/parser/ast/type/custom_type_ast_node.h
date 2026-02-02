#pragma once

#include "parser/ast/impl/i_resolvable.h"
#include "type_ast_node.h"

#include <optional>
#include <string>

namespace codesh::semantic_analyzer
{
class type_symbol;
}
namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class custom_type_ast_node final : public type_ast_node, public impl::i_resolvable<semantic_analyzer::type_symbol>
{
    const definition::fully_qualified_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> resolved_symbol;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> &_get_resolved()
        const override;

public:
    custom_type_ast_node(blasphemy::code_position code_position, definition::fully_qualified_name name);

    void set_resolved(semantic_analyzer::type_symbol &symbol) override;

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] const definition::fully_qualified_name &get_unresolved_name() const override;

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;
    [[nodiscard]] std::string to_pretty_string() const override;


    [[nodiscard]] output::ir::instruction_type to_instruction_type() const override;
};

}
