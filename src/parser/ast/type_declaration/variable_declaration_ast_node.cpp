#include "variable_declaration_ast_node.h"
#include "output/jvm_target/constant_pool.h"
#include "parser/ast/type/custom_type_ast_node.h"


codesh::ast::type_decl::variable_declaration_ast_node::variable_declaration_ast_node(
        const blasphemy::code_position pos) :
    ast_node(pos)
{
}

const std::string& codesh::ast::type_decl::variable_declaration_ast_node::get_name() const
{
    return name;
}

void codesh::ast::type_decl::variable_declaration_ast_node::set_name(const std::string& name)
{
    this->name = name;
}

codesh::ast::type::type_ast_node* codesh::ast::type_decl::variable_declaration_ast_node::get_type() const
{
    return type.get();
}

void codesh::ast::type_decl::variable_declaration_ast_node::set_type(std::unique_ptr<type::type_ast_node> type)
{
    this->type = std::move(type);
}

codesh::ast::type_decl::attributes_ast_node* codesh::ast::type_decl::variable_declaration_ast_node::get_attributes() const
{
    return attributes.get();
}

void codesh::ast::type_decl::variable_declaration_ast_node::set_attributes(
        std::unique_ptr<attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

void codesh::ast::type_decl::variable_declaration_ast_node::emit_constants(const compilation_unit_ast_node& root_node,
        output::jvm_target::constant_pool& constant_pool)
{
    constant_pool.goc_utf8_info(get_name());
    const int descriptor_cpi = constant_pool.goc_utf8_info(get_type()->generate_descriptor());

    // For object and array types, also add a Class entry so it can be referenced
    // in the StackMapTable entries (verification):
    if (get_type()->get_array_dimensions() != 0)
    {
        constant_pool.goc_class_info(descriptor_cpi);
    }
    else if (const auto custom_type = dynamic_cast<const type::custom_type_ast_node *>(get_type()))
    {
        constant_pool.goc_class_info(
            constant_pool.goc_utf8_info(
                custom_type->get_resolved_name().join()
            )
        );
    }
}
