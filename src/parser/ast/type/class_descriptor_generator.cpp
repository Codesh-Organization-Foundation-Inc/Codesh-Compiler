#include "class_descriptor_generator.h"

codesh::util::class_descriptor_generator::class_descriptor_generator(
    ast::type_decl::type_declaration_ast_node *const type_declaration)
    : type_declaration(type_declaration)
{
}

std::string codesh::util::class_descriptor_generator::generate_descriptor() const
{
    //TODO: Add package here
    return "L" + this->type_declaration->get_name() + ";";
}