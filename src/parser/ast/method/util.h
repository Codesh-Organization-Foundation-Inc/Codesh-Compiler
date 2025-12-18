#pragma once
#include "../type_declaration/type_declaration_ast_node.h"

#include <memory>
#include <string>

namespace codesh::ast::method::util
{
[[nodiscard]] std::string generate_descriptor(bool resolved, const type::type_ast_node &return_type,
        const std::vector<std::reference_wrapper<type::type_ast_node>> &parameter_types,
        const type_decl::attributes_ast_node &attributes);

[[nodiscard]] std::string generate_parameters_descriptor(bool resolved,
        const std::vector<std::reference_wrapper<type::type_ast_node>> &parameter_types,
        const type_decl::attributes_ast_node &attributes);
}
