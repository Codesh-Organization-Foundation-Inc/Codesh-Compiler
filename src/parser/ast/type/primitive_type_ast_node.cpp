#include "primitive_type_ast_node.h"

#include "../../../lexer/trie/keywords.h"
#include "../../../output/ir/instruction.h"
#include "fmt/format.h"

#include <sstream>
#include <stdexcept>

codesh::ast::type::primitive_type_ast_node::primitive_type_ast_node(const definition::primitive_type type) : type(type)
{
}

std::string codesh::ast::type::primitive_type_ast_node::generate_descriptor(bool resolved) const
{
    char type;
    switch (get_type())
    {
    case definition::primitive_type::VOID: type = 'V'; break;
    case definition::primitive_type::FLOAT: type = 'F'; break;
    case definition::primitive_type::CHAR: type = 'C'; break;
    case definition::primitive_type::DOUBLE: type = 'D'; break;
    case definition::primitive_type::INTEGER: type = 'I'; break;
    case definition::primitive_type::LONG: type = 'J'; break;
    case definition::primitive_type::SHORT: type = 'S'; break;
    case definition::primitive_type::BYTE: type = 'B'; break;
    case definition::primitive_type::BOOLEAN: type = 'Z'; break;

    default: throw std::runtime_error("Unknown primitive type");
    }

    return fmt::format(
        "{}{}",
        std::string(get_array_dimensions(), '['),
        type
    );
}


codesh::definition::primitive_type codesh::ast::type::primitive_type_ast_node::get_type() const
{
    return type;
}

codesh::output::ir::instruction_type codesh::ast::type::primitive_type_ast_node::to_instruction_type() const
{
    switch (get_type())
    {
    case definition::primitive_type::FLOAT:
        return output::ir::instruction_type::FLOAT;
    case definition::primitive_type::DOUBLE:
        return output::ir::instruction_type::DOUBLE;
    case definition::primitive_type::LONG:
        return output::ir::instruction_type::LONG;

    case definition::primitive_type::INTEGER:
    case definition::primitive_type::SHORT:
    case definition::primitive_type::BYTE:
    case definition::primitive_type::BOOLEAN:
    case definition::primitive_type::CHAR:
        return output::ir::instruction_type::INT;

    case definition::primitive_type::VOID:
        throw std::runtime_error("Void has no instruction type");

    default:
        throw std::runtime_error("Unknown instruction type");
    }

}

std::unique_ptr<codesh::ast::type::type_ast_node> codesh::ast::type::primitive_type_ast_node::clone() const
{
    return std::make_unique<primitive_type_ast_node>(*this);
}

std::string codesh::ast::type::primitive_type_ast_node::to_pretty_string() const
{
    switch (get_type())
    {
    case definition::primitive_type::INTEGER:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_INTEGER);
    case definition::primitive_type::DOUBLE:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_DOUBLE);
    case definition::primitive_type::FLOAT:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_FLOAT);
    case definition::primitive_type::LONG:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_LONG);
    case definition::primitive_type::SHORT:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_SHORT);
    case definition::primitive_type::BYTE:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_BYTE);
    case definition::primitive_type::CHAR:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_CHAR);
    case definition::primitive_type::BOOLEAN:
        return lexer::trie::TOKEN_TO_NAME_MAP.at(token_group::KEYWORD_BOOLEAN);

    default:
        throw std::runtime_error("Cannot pretty print the current type");
    }
}
