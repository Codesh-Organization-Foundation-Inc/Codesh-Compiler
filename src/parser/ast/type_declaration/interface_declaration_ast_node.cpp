#include "interface_declaration_ast_node.h"

#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/builtins.h"

interface_declaration_ast_node::interface_declaration_ast_node(codesh::lexer::code_position code_position,
                                                               codesh::definition::fully_qualified_name name) :
    type_declaration_ast_node(code_position, std::move(name))
{
    set_super_class(
        std::make_unique<codesh::ast::type::custom_type_ast_node>(
            code_position,
            codesh::definition::fully_qualified_name::parse(
                codesh::semantic_analyzer::builtins::ALIAS_OBJECT,
                code_position
            )
        )
    );
}
