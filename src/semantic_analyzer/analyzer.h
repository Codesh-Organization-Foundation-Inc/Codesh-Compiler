#pragma once

#include "../parser/ast/compilation_unit_ast_node.h"
#include "../parser/ast/local_variable_declaration_ast_node.h"


namespace codesh::semantic_analyzer
{

    static void run(ast::compilation_unit_ast_node & root);
    static void add_default_constructors(const ast::compilation_unit_ast_node &root_node);
    static void add_this_param_to_non_static_methods(const ast::compilation_unit_ast_node &root_node);
    static std::unique_ptr<ast::local_variable_declaration_ast_node> create_this_param(
        const ast::type_decl::class_declaration_ast_node &class_decl);

}