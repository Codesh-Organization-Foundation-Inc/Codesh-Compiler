#include "constructor_declaration_ast_node.h"

#include "defenition/definitions.h"
#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::method::constructor_declaration_ast_node::constructor_declaration_ast_node(
        const lexer::code_position code_position, std::unique_ptr<type::custom_type_ast_node> constructed_type) :
    method_declaration_ast_node(
        code_position,
        definition::fully_qualified_name(lexer::NO_CODE_POS, definition::JVM_CONSTRUCTOR_NAME)
    ),
    constructed_type(std::move(constructed_type))
{
    set_return_type(std::make_unique<type::primitive_type_ast_node>(code_position, definition::primitive_type::VOID));
}

codesh::ast::type::custom_type_ast_node &codesh::ast::method::constructor_declaration_ast_node::get_constructed_type()
    const
{
    return *constructed_type;
}

std::string codesh::ast::method::constructor_declaration_ast_node::to_pretty_string() const
{
    return fmt::format(
        definition::CONSTRUCTOR_PRETTY_STRING,
        constructed_type->to_pretty_string()
    );
}
