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

    /**
     * A type declaration reference that exists if the 2nd constructor was used.
     *
     * Simplifies the resolve process.
     */
    std::optional<std::reference_wrapper<ast::type_decl::type_declaration_ast_node>> known_type_declaration;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::type_symbol>> &_get_resolved()
        const override;

public:
    custom_type_ast_node(blasphemy::code_position code_position, definition::fully_qualified_name name);
    custom_type_ast_node(blasphemy::code_position code_position, ast::type_decl::type_declaration_ast_node &type_decl);

    void set_resolved(semantic_analyzer::type_symbol &symbol) override;

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] const definition::fully_qualified_name &get_unresolved_name() const override;

    [[nodiscard]] std::unique_ptr<type_ast_node> clone() const override;
    [[nodiscard]] std::string to_pretty_string() const override;

    /**
     * A type declaration reference that exists if the 2nd constructor was used.
     *
     * Simplifies the resolve process.
     *
     * If this returns `std::nullopt`, the regular resolve process should be used.
     */
    [[nodiscard]] std::optional<std::reference_wrapper<ast::type_decl::type_declaration_ast_node>>
        get_known_type_declaration() const;

    [[nodiscard]] output::ir::instruction_type to_instruction_type() const override;
};

}
