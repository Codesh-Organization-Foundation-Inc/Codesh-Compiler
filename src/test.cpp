#include "test.h"

#include "defenition/primitive_type.h"
#include "parser/ast/method/method_declaration_ast_node.h"
#include "parser/ast/local_variable_declaration_ast_node.h"
#include "parser/ast/type/primitive_type_ast_node.h"

#include <iostream>

void codesh::test::descriptor()
{
    ast::method::method_declaration_ast_node method;

    // Normal return type
    method.set_return_type(std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::BYTE));
    std::cout << method.generate_descriptor() << std::endl;


    // Adding a parameter
    std::unique_ptr<ast::local_variable_declaration_ast_node> local_var = std::make_unique<ast::local_variable_declaration_ast_node>();

    std::unique_ptr<ast::type::primitive_type_ast_node> param_type = std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::INTEGER);
    param_type->set_array_dimensions(2);

    local_var->set_type(std::move(param_type));
    method.get_parameters().push_back(std::move(local_var));

    std::cout << method.generate_descriptor() << std::endl;

    // And another
    std::unique_ptr<ast::local_variable_declaration_ast_node> local_var_2 = std::make_unique<ast::local_variable_declaration_ast_node>();
    local_var_2->set_type(std::make_unique<ast::type::primitive_type_ast_node>(definition::primitive_type::FLOAT));

    method.get_parameters().push_back(std::move(local_var_2));

    std::cout << method.generate_descriptor() << std::endl;
}