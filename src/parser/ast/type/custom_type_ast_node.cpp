#include "custom_type_ast_node.h"

#include "../../../output/ir/instruction.h"
#include "../../../semantic_analyzer/symbol_table/symbol.h"
#include "fmt/chrono.h"

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> &codesh::ast::type::
    custom_type_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

codesh::ast::type::custom_type_ast_node::custom_type_ast_node(definition::fully_qualified_class_name name) :
    name(std::move(name))
{
}

void codesh::ast::type::custom_type_ast_node::set_resolved(semantic_analyzer::type_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

std::string codesh::ast::type::custom_type_ast_node::generate_descriptor(const bool resolved) const
{
    return fmt::format(
        "{}L{};",
        std::string(get_array_dimensions(), '['),
        get_name(resolved).join()
    );
}

const codesh::definition::fully_qualified_class_name &codesh::ast::type::custom_type_ast_node::get_unresolved_name() const
{
    return name;
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::custom_type_ast_node::clone() const
{
    return std::make_unique<custom_type_ast_node>(*this);
}

std::string codesh::ast::type::custom_type_ast_node::to_pretty_string() const
{
    const definition::fully_qualified_class_name &fqcn = resolved_symbol.has_value()
        ? get_resolved().get_full_name()
        : get_unresolved_name();

    return fqcn.holy_join();
}

codesh::output::ir::instruction_type codesh::ast::type::custom_type_ast_node::to_instruction_type() const
{
    return output::ir::instruction_type::REFERENCE;
}