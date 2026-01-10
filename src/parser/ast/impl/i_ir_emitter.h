#pragma once

#include "ast_node.h"

namespace codesh::ast::type_decl
{
class type_declaration_ast_node;
}
namespace codesh::semantic_analyzer
{
class symbol_table;
}
namespace codesh::output::ir
{
class code_block;
}


namespace codesh::ast::impl
{

/**
 * An object that can emit method IR
 */
class i_ir_emitter
{
    /**
     * @return The IR representation of this AST node
     */
public:
    virtual ~i_ir_emitter() = default;

    virtual void emit_ir(output::ir::code_block &containing_block, const semantic_analyzer::symbol_table &symbol_table,
                       const type_decl::type_declaration_ast_node &containing_type_decl) const = 0;
};

}

