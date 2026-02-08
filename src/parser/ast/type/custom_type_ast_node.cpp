#include "custom_type_ast_node.h"

#include "lexer/trie/keywords.h"
#include "output/ir/instruction/impl/typed_instruction.h"
#include "parser/ast/type_declaration/type_declaration_ast_node.h"
#include "semantic_analyzer/symbol_table/symbol.h"

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> &codesh::ast::type::
    custom_type_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

codesh::ast::type::custom_type_ast_node::custom_type_ast_node(const blasphemy::code_position code_position,
        definition::fully_qualified_name name) :
    type_ast_node(code_position),
    name(std::move(name))
{
}

codesh::ast::type::custom_type_ast_node::custom_type_ast_node(const blasphemy::code_position code_position,
                                                              ast::type_decl::type_declaration_ast_node &type_decl) :
    type_ast_node(code_position),
    name(type_decl.get_unresolved_name()),
    known_type_declaration(type_decl)
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

const codesh::definition::fully_qualified_name &codesh::ast::type::custom_type_ast_node::get_unresolved_name() const
{
    return name;
}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::custom_type_ast_node::clone() const
{
    return std::make_unique<custom_type_ast_node>(*this);
}

std::string codesh::ast::type::custom_type_ast_node::to_pretty_string() const
{
    const definition::fully_qualified_name &fqn = resolved_symbol.has_value()
        ? get_resolved().get_full_name()
        : get_unresolved_name();

    if (fqn.join() == "java/lang/String")
        return lexer::trie::keyword::ALIAS_STRING;

    return fqn.holy_join();
}

std::optional<std::reference_wrapper<codesh::ast::type_decl::type_declaration_ast_node>> codesh::ast::type::
    custom_type_ast_node::get_known_type_declaration() const
{
    return known_type_declaration;
}

codesh::output::ir::instruction_type codesh::ast::type::custom_type_ast_node::to_instruction_type() const
{
    return output::ir::instruction_type::REFERENCE;
}