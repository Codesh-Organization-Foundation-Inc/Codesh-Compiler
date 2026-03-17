#include "attributes_ast_node.h"

#include "output/jvm_target/class_file_builder.h"

codesh::ast::type_decl::attributes_ast_node::attributes_ast_node(const lexer::code_position code_position) :
    ast_node(code_position),
    visibility(definition::visibility::PACKAGE_PRIVATE),
    is_static(false),
    is_final(false),
    is_abstract(false)
{
}

std::unique_ptr<codesh::ast::type_decl::attributes_ast_node> codesh::ast::type_decl::attributes_ast_node::clone() const
{
    return std::make_unique<attributes_ast_node>(*this);
}

std::vector<codesh::output::jvm_target::access_flag> codesh::ast::type_decl::attributes_ast_node::get_access_flags() const
{
    std::vector<output::jvm_target::access_flag> result;

    switch (visibility)
    {
    case definition::visibility::PUBLIC:
        result.push_back(output::jvm_target::access_flag::ACC_PUBLIC);
        break;
    case definition::visibility::PRIVATE:
        result.push_back(output::jvm_target::access_flag::ACC_PRIVATE);
        break;
    case definition::visibility::PROTECTED:
        result.push_back(output::jvm_target::access_flag::ACC_PROTECTED);
        break;

    default: break;
    }

    if (is_static)
    {
        result.push_back(output::jvm_target::access_flag::ACC_STATIC);
    }
    if (is_final)
    {
        result.push_back(output::jvm_target::access_flag::ACC_FINAL);
    }
    if (is_abstract)
    {
        result.push_back(output::jvm_target::access_flag::ACC_ABSTRACT);
    }

    return result;
}

codesh::definition::visibility codesh::ast::type_decl::attributes_ast_node::get_visibility() const
{
    return visibility;
}
void codesh::ast::type_decl::attributes_ast_node::set_visibility(const definition::visibility visibility)
{
    this->visibility = visibility;
}

bool codesh::ast::type_decl::attributes_ast_node::get_is_static() const
{
    return is_static;
}
void codesh::ast::type_decl::attributes_ast_node::set_is_static(const bool is_static)
{
    this->is_static = is_static;
}

bool codesh::ast::type_decl::attributes_ast_node::get_is_final() const
{
    return is_final;
}
void codesh::ast::type_decl::attributes_ast_node::set_is_final(const bool is_final)
{
    this->is_final = is_final;
}

bool codesh::ast::type_decl::attributes_ast_node::get_is_abstract() const
{
    return is_abstract;
}
void codesh::ast::type_decl::attributes_ast_node::set_is_abstract(const bool is_abstract)
{
    this->is_abstract = is_abstract;
}
