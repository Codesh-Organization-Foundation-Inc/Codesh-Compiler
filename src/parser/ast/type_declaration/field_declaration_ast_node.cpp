#include "field_declaration_ast_node.h"
#include "type_declaration_ast_node.h"
#include "output/jvm_target/constant_pool.h"
#include "parser/ast/impl/i_constant_pool_emitter.h"

#include <assert.h>

codesh::ast::type_decl::field_declaration_ast_node::field_declaration_ast_node(const lexer::code_position pos) :
        variable_declaration_ast_node(pos)
{
}

codesh::ast::var_reference::value_ast_node* codesh::ast::type_decl::field_declaration_ast_node::get_value() const
{
    return value.get();
}

void codesh::ast::type_decl::field_declaration_ast_node::set_value(std::unique_ptr<var_reference::value_ast_node> val)
{
    value = std::move(val);
}

void codesh::ast::type_decl::field_declaration_ast_node::set_containing_type(type_declaration_ast_node &type)
{
    containing_type = &type;
}

std::optional<int> codesh::ast::type_decl::field_declaration_ast_node::get_field_cpi() const
{
    return field_cpi;
}

void codesh::ast::type_decl::field_declaration_ast_node::emit_constants(
        const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool)
{
    variable_declaration_ast_node::emit_constants(root_node, constant_pool);

    if (value == nullptr || !get_attributes()->get_is_static())
        return;

    constant_pool.goc_utf8_info("<clinit>");
    constant_pool.goc_utf8_info("()V");


    if (const auto cp_emitter = dynamic_cast<i_constant_pool_emitter *>(value.get()))
    {
        cp_emitter->emit_constants(root_node, constant_pool);
    }

    assert(containing_type != nullptr && "Fields must be contained within types");

    field_cpi = constant_pool.goc_fieldref_info(
        constant_pool.goc_class_info(
            constant_pool.goc_utf8_info(containing_type->get_resolved_name().join())
        ),
        constant_pool.goc_name_and_type_info(
            constant_pool.goc_utf8_info(get_name()),
            constant_pool.goc_utf8_info(generate_descriptor(true))
        )
    );
}

std::string codesh::ast::type_decl::field_declaration_ast_node::generate_descriptor(const bool resolved) const
{
    return get_type()->generate_descriptor(resolved);
}
