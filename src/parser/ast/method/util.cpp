#include "util.h"

#include "fmt/xchar.h"

#include <ranges>

[[nodiscard]] static std::string generate_unresolved_parameter_descriptors(
        const std::deque<std::reference_wrapper<codesh::ast::type::type_ast_node>> &parameter_types);


std::string codesh::ast::method::util::generate_method_descriptor(
        const std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol,
        const type::type_ast_node &return_type,
        const std::deque<std::reference_wrapper<type::type_ast_node>> &parameter_types,
        const type_decl::attributes_ast_node &attributes)
{
    // In JVM, a method descriptor is defined as such:
    // (args...)return_type

    return fmt::format(
        "({}){}",
        generate_parameters_descriptor(resolved_symbol, parameter_types, attributes),
        return_type.generate_descriptor(resolved_symbol.has_value())
    );
}

std::string codesh::ast::method::util::generate_parameters_descriptor(
        const std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol,
        const std::deque<std::reference_wrapper<type::type_ast_node>> &parameter_types,
        const type_decl::attributes_ast_node &attributes)
{
    if (!resolved_symbol)
        return generate_unresolved_parameter_descriptors(parameter_types);


    fmt::memory_buffer out;
    const auto &param_types = parameter_types;

    auto it = param_types.begin();

    // If the method isn't static, skip the first parameter ('this').
    // NOTE that external symbols do NOT have 'this' prepended.
    const auto is_external = resolved_symbol->get().get_producing_node() == nullptr;
    if (!is_external && !attributes.get_is_static())
    {
        ++it;
    }

    for (; it != param_types.end(); ++it)
    {
        const auto &param = *it;

        fmt::format_to(
            std::back_inserter(out),
            "{}",
            param.get().generate_descriptor()
        );
    }

    return fmt::to_string(out);
}

static std::string generate_unresolved_parameter_descriptors(
        const std::deque<std::reference_wrapper<codesh::ast::type::type_ast_node>> &parameter_types)
{
    auto descriptors = parameter_types
        | std::views::transform([](const codesh::ast::type::type_ast_node &var_type) {
            return var_type.generate_descriptor(false);
        });

    return fmt::format(
        "{}",
        fmt::join(descriptors, "")
    );
}