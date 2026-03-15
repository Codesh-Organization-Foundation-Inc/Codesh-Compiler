#pragma once

#include "type_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

class error_type_declaration_ast_node final : public type_declaration_ast_node
{
public:
    explicit error_type_declaration_ast_node(blasphemy::code_position code_position);

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) override;
};

}
