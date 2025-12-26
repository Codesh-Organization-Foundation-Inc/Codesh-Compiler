#include "type_declaration_ast_node.h"

#include "../../../defenition/definitions.h"
#include "../../../semantic_analyzer/symbol_table/symbol.h"
#include "../compilation_unit_ast_node.h"
#include "../type/custom_type_ast_node.h"

#include <utility>

const std::optional<std::reference_wrapper<codesh::semantic_analyzer::type_symbol>> &codesh::ast::type_decl::
    type_declaration_ast_node::_get_resolved() const
{
    return resolved_symbol;
}

codesh::ast::type_decl::type_declaration_ast_node::type_declaration_ast_node(
        definition::fully_qualified_class_name name) : name(std::move(name))
{
}
codesh::ast::type_decl::type_declaration_ast_node::~type_declaration_ast_node() = default;

void codesh::ast::type_decl::type_declaration_ast_node::set_resolved(semantic_analyzer::type_symbol &symbol)
{
    resolved_symbol.emplace(symbol);
}

std::string codesh::ast::type_decl::type_declaration_ast_node::generate_descriptor(const bool resolved) const
{
    return "L" + get_name(resolved).join() + ";";
}

const codesh::definition::fully_qualified_class_name &codesh::ast::type_decl::type_declaration_ast_node::get_unresolved_name()
    const
{
    return this->name;
}

codesh::ast::type::custom_type_ast_node *codesh::ast::type_decl::type_declaration_ast_node::get_super_class() const
{
    return super_class.get();
}

void codesh::ast::type_decl::type_declaration_ast_node::set_super_class(
    std::unique_ptr<type::custom_type_ast_node> super_class)
{
    this->super_class = std::move(super_class);
}

codesh::ast::type_decl::attributes_ast_node *codesh::ast::type_decl::type_declaration_ast_node::get_attributes()
    const
{
    return this->attributes.get();
}
void codesh::ast::type_decl::type_declaration_ast_node::set_attributes(
    std::unique_ptr<attributes_ast_node> attributes)
{
    this->attributes = std::move(attributes);
}

const codesh::output::jvm_target::constant_pool &codesh::ast::type_decl::
    type_declaration_ast_node::get_constant_pool() const
{
    if (constant_pool == nullptr)
        throw std::runtime_error("Attempted to get constant pool in type, yet it was uninitialized");

    return *constant_pool;
}

void codesh::ast::type_decl::type_declaration_ast_node::set_constant_pool(
        output::jvm_target::constant_pool constant_pool)
{
    this->constant_pool = std::make_unique<output::jvm_target::constant_pool>(std::move(constant_pool));
}

const std::list<std::unique_ptr<codesh::ast::method::method_declaration_ast_node>> &codesh::ast::type_decl::
    type_declaration_ast_node::get_all_methods() const
{
    return all_methods;
}

void codesh::ast::type_decl::type_declaration_ast_node::add_method(
    std::unique_ptr<method::method_declaration_ast_node> method)
{
    all_methods.push_back(std::move(method));
    methods.push_back(all_methods.back().get());
}

void codesh::ast::type_decl::type_declaration_ast_node::add_method(
    std::unique_ptr<method::constructor_declaration_ast_node> method)
{
    all_methods.push_front(std::move(method));
    constructors.push_back(static_cast<method::constructor_declaration_ast_node *>(all_methods.front().get())); // NOLINT(*-pro-type-static-cast-downcast)
}

const std::list<codesh::ast::method::constructor_declaration_ast_node *> &codesh::ast::type_decl::
    type_declaration_ast_node::get_constructors() const
{
    return constructors;
}

const std::list<codesh::ast::method::method_declaration_ast_node *> &codesh::ast::type_decl::type_declaration_ast_node::
    get_methods() const
{
    return methods;
}

void codesh::ast::type_decl::type_declaration_ast_node::emit_constants(
        const compilation_unit_ast_node &root_node, output::jvm_target::constant_pool &constant_pool)
{
    emit_metadata(root_node, constant_pool);

    // This class
    constant_pool.goc_class_info(
        constant_pool.goc_utf8_info(get_resolved_name().join())
    );
    constant_pool.goc_utf8_info(generate_descriptor());

    // Superclass
    const int super_class_cpi = constant_pool.goc_utf8_info(
        get_super_class()->get_resolved_name().join()
    );
    const int super_class_constant = constant_pool.goc_class_info(super_class_cpi);

    // Add super constructor method reference
    //TODO: Move to IR
    constant_pool.goc_methodref_info(
        super_class_constant,

        constant_pool.goc_name_and_type_info(
            constant_pool.goc_utf8_info("<init>"),
            //TODO: Actually check super call params
            constant_pool.goc_utf8_info("()V")
        )
    );

    // Emit methods
    for (const auto &method_decl : get_all_methods())
    {
        method_decl->emit_constants(root_node, constant_pool);
    }
}

void codesh::ast::type_decl::type_declaration_ast_node::emit_metadata(const compilation_unit_ast_node &root_node,
        output::jvm_target::constant_pool &constant_pool) const
{
    // Filename
    constant_pool.goc_utf8_info(root_node.get_source_stem() + definition::SOURCE_FILE_EXTENSION);

    // More bs
    constant_pool.goc_utf8_info("SourceFile");
    constant_pool.goc_utf8_info("Code");
    constant_pool.goc_utf8_info("LocalVariableTable");
}
