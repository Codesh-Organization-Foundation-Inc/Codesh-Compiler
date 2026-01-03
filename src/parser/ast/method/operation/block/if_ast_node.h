#pragma once
#include "../method_operation_ast_node.h"

namespace codesh::ast::method
{
class method_scope_ast_node;
}
namespace codesh::ast::block // TODO: do it like - block::logic
{

class if_ast_node : public method::operation::method_operation_ast_node
{
    method::method_scope_ast_node &method_scopes;

public:
    explicit if_ast_node(method::method_scope_ast_node method_scope);

    void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};
}