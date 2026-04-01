#pragma once
#include "parser/ast/type_declaration/type_declaration_ast_node.h"

#include <string>

namespace codesh::ast::method::util
{
[[nodiscard]] std::string generate_method_descriptor(
        std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol,
        const type::type_ast_node &return_type,
        const std::deque<std::reference_wrapper<type::type_ast_node>> &parameter_types,
        const type_decl::attributes_ast_node &attributes);

[[nodiscard]] std::string generate_parameters_descriptor(
        std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol,
        const std::deque<std::reference_wrapper<type::type_ast_node>> &parameter_types,
        const type_decl::attributes_ast_node &attributes);

[[nodiscard]] std::string pretty_method_signature(const semantic_analyzer::method_symbol &method);
}
