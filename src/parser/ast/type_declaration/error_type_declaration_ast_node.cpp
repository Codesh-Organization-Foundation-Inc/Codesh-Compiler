#include "error_type_declaration_ast_node.h"

#include <stdexcept>

codesh::ast::type_decl::error_type_declaration_ast_node::error_type_declaration_ast_node() :
    type_declaration_ast_node(definition::fully_qualified_name{})
{
}

void codesh::ast::type_decl::error_type_declaration_ast_node::emit_constants(
        const compilation_unit_ast_node &root_node, output::jvm_target::constant_pool &constant_pool)
{
    throw std::runtime_error("Attempted to emit constants for an errored type declaration");
}
