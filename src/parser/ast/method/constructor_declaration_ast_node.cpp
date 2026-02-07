#include "constructor_declaration_ast_node.h"

#include "defenition/definitions.h"
#include "parser/ast/type/primitive_type_ast_node.h"

codesh::ast::method::constructor_declaration_ast_node::constructor_declaration_ast_node(
        const blasphemy::code_position code_position) :
    method_declaration_ast_node(code_position, definition::fully_qualified_name(definition::JVM_CONSTRUCTOR_NAME))
{
    set_return_type(std::make_unique<type::primitive_type_ast_node>(code_position, definition::primitive_type::VOID));
}

std::string codesh::ast::method::constructor_declaration_ast_node::to_pretty_string() const
{
    return fmt::format(
        definition::CONSTRUCTOR_PRETTY_STRING,
        get_last_name(false)
    );
}
