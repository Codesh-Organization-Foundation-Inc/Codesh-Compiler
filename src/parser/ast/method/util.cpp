#include "util.h"

#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

#include "fmt/xchar.h"

#include <ranges>

[[nodiscard]] static std::string generate_unresolved_parameter_descriptors(
        const std::deque<std::reference_wrapper<codesh::ast::type::type_ast_node>> &parameter_types);
[[nodiscard]] static std::string generate_signature_base_name(const codesh::semantic_analyzer::method_symbol &method);
[[nodiscard]] static std::optional<std::string> generate_signature_params(
        const codesh::semantic_analyzer::method_symbol &method);
[[nodiscard]] static std::string generate_params_with_names(
        const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &parameter_types,
        const codesh::ast::method::method_declaration_ast_node &producing_node, bool is_static);
[[nodiscard]] static std::string generate_params_types_only(
        const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &parameter_types, bool is_static);
[[nodiscard]] static bool has_non_void_return(const codesh::ast::type::type_ast_node &return_type);


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
    if (!resolved_symbol->get().is_external() && !attributes.get_is_static())
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


std::string codesh::ast::method::util::pretty_method_signature(const semantic_analyzer::method_symbol &method)
{
    std::string signature = generate_signature_base_name(method);

    if (const auto params = generate_signature_params(method))
    {
        signature += *params;
    }

    if (has_non_void_return(method.get_return_type()))
    {
        signature += fmt::format(" וַיָּשָ֨ב {}", method.get_return_type().to_pretty_string());
    }

    return signature;
}

static std::string generate_signature_base_name(const codesh::semantic_analyzer::method_symbol &method)
{
    return fmt::format("מַעֲשֶׂה וּשְׁמוֹ {}", method.get_full_name().get_last_part());
}

static std::optional<std::string> generate_signature_params(const codesh::semantic_analyzer::method_symbol &method)
{
    const auto &parameter_types = method.get_parameter_types();
    if (parameter_types.empty())
        return std::nullopt;

    const auto *producing_node = method.get_producing_node();
    const bool is_static = method.get_attributes().get_is_static();

    if (producing_node == nullptr)
        return generate_params_types_only(parameter_types, is_static);

    return generate_params_with_names(parameter_types, *producing_node, is_static);
}

static std::string generate_params_with_names(
        const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &parameter_types,
        const codesh::ast::method::method_declaration_ast_node &producing_node, const bool is_static)
{
    std::vector<std::string> param_strings;
    for (size_t i = 0; i < parameter_types.size(); ++i)
    {
        if (i == 0 && !is_static)
            continue;

        param_strings.push_back(fmt::format(
            "{} וּשְׁמוֹ {}",
            parameter_types.at(i)->to_pretty_string(),
            producing_node.get_parameters().at(i).get().get_name()
        ));
    }

    return fmt::format(" וַיִּקַּח {}", fmt::join(param_strings, " וַיִּקַּח "));
}

static std::string generate_params_types_only(
        const std::vector<std::unique_ptr<codesh::ast::type::type_ast_node>> &parameter_types, const bool is_static)
{
    std::vector<std::string> type_strings;
    for (size_t i = 0; i < parameter_types.size(); ++i)
    {
        if (i == 0 && !is_static)
            continue;

        type_strings.push_back(parameter_types.at(i)->to_pretty_string());
    }

    return fmt::format("{}", fmt::join(type_strings, " וְ־ "));
}

static bool has_non_void_return(const codesh::ast::type::type_ast_node &return_type)
{
    const auto *prim = dynamic_cast<const codesh::ast::type::primitive_type_ast_node *>(&return_type);
    return prim == nullptr || prim->get_type() != codesh::definition::primitive_type::VOID;
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
