#pragma once

#include "parser/ast/var_reference/value_ast_node.h"
#include "parser/ast/type/type_ast_node.h"


namespace codesh::ast::op
{

class new_ast_node final :  public var_reference::value_ast_node
{
    std::unique_ptr<type::type_ast_node> constructed_type;
    std::vector<std::unique_ptr<value_ast_node>> arguments;

public:
    new_ast_node(
        blasphemy::code_position code_position,
        std::unique_ptr<type::type_ast_node> constructed_type
    );

    [[nodiscard]] type::type_ast_node &get_constructed_type();
    [[nodiscard]] const type::type_ast_node &get_constructed_type() const;


    [[nodiscard]] std::vector<std::unique_ptr<value_ast_node>> &get_arguments();

    void emit_ir(output::ir::code_block &containing_block,
                 const semantic_analyzer::symbol_table &symbol_table,
                 const type_decl::type_declaration_ast_node &containing_type_decl) const override;
};

}
