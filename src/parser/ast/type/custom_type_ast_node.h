#pragma once

#include "../impl/i_resolvable.h"
#include "type_ast_node.h"
#include "../type_declaration/type_declaration_ast_node.h"

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


class custom_type_ast_node : public type_ast_node, public impl::i_resolvable<semantic_analyzer::type_symbol>
{
    const definition::fully_qualified_class_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> resolved_symbol;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> &_get_resolved()
        const override;

public:
    explicit custom_type_ast_node(definition::fully_qualified_class_name name);

    void set_resolved(semantic_analyzer::type_symbol &symbol) override;

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] const definition::fully_qualified_class_name &get_unresolved_name() const override;

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;


    [[nodiscard]] output::ir::instruction_type to_instruction_type() const override;
};

}
