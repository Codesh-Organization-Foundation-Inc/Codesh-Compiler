#include "field_declaration_ast_node.h"

namespace codesh::ast::type_decl
{

    field_declaration_ast_node::field_declaration_ast_node(blasphemy::code_position pos,
            definition::fully_qualified_name name, std::unique_ptr<type::type_ast_node> field_type) :
        ast_node(pos),
        name(std::move(name)),
        field_type(std::move(field_type))
    {
    }

    const definition::fully_qualified_name& field_declaration_ast_node::get_name() const
    {
        return name;
    }

    type::type_ast_node* field_declaration_ast_node::get_type() const
    {
        return field_type.get();
    }

    attributes_ast_node* field_declaration_ast_node::get_attributes() const
    {
        return attributes.get();
    }

    void field_declaration_ast_node::set_attributes(std::unique_ptr<attributes_ast_node> attributes)
    {
        this->attributes = std::move(attributes);
    }

    var_reference::value_ast_node* field_declaration_ast_node::get_value() const
    {
        return value.get();
    }

    void field_declaration_ast_node::set_value(std::unique_ptr<var_reference::value_ast_node> val)
    {
        value = std::move(val);
    }

    std::string field_declaration_ast_node::generate_descriptor(bool resolved) const
    {
        return field_type->generate_descriptor(resolved);
    }

    void field_declaration_ast_node::emit_constants(const compilation_unit_ast_node&,
        output::jvm_target::constant_pool& constant_pool)
    {
    }

}