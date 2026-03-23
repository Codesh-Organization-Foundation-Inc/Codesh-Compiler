#include "interface_declaration_ast_node.h"

#include "parser/ast/type/custom_type_ast_node.h"
#include "semantic_analyzer/builtins.h"

interface_declaration_ast_node::interface_declaration_ast_node(codesh::lexer::code_position code_position,
                                                               codesh::definition::fully_qualified_name name) :
    type_declaration_ast_node(code_position, std::move(name))
{
    auto object_fqn = codesh::definition::fully_qualified_name::parse(
        codesh::semantic_analyzer::builtins::ALIAS_OBJECT, code_position);

    auto custom_type_node = std::make_unique<codesh::ast::type::custom_type_ast_node>(code_position, object_fqn);

    set_super_class(std::move(custom_type_node));
}
