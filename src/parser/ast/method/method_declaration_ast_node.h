#pragma once

#include "../../../output/jvm_target/class_file_builder.h"
#include "../impl/ast_node.h"
#include "../impl/i_descriptor_emitter.h"
#include "../impl/ir_convertable_ast_node.h"
#include "../local_variable_declaration_ast_node.h"
#include "../type/type_ast_node.h"
#include "../type_declaration/attributes_ast_node.h"

#include <list>
#include <memory>
#include <string>

namespace codesh::ast
{

class method_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter
{
    std::string name;
    std::unique_ptr<type_decl::attributes_ast_node> attributes;

    std::unique_ptr<type::type_ast_node> return_type;
    std::list<std::unique_ptr<local_variable_declaration_ast_node>> parameters;

    // "throws" declaration
    std::list<std::unique_ptr<type::type_ast_node>> exceptions_thrown;


    std::list<std::unique_ptr<impl::ir_convertable_ast_node>> body;


    [[nodiscard]] std::string generate_unresolved_parameter_descriptors() const;

public:
    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] std::string generate_parameters_descriptor(bool resolved = true) const;


    [[nodiscard]] std::string get_name() const;
    void set_name(const std::string &name);

    [[nodiscard]] type_decl::attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<type_decl::attributes_ast_node> attributes);

    [[nodiscard]] type::type_ast_node *get_return_type() const;
    void set_return_type(std::unique_ptr<type::type_ast_node> return_type);


    [[nodiscard]] const std::list<std::unique_ptr<local_variable_declaration_ast_node>> &get_parameters() const;
    [[nodiscard]] const std::list<std::unique_ptr<type::type_ast_node>> &get_exceptions_thrown() const;

    [[nodiscard]] std::list<std::unique_ptr<local_variable_declaration_ast_node>> &get_parameters();
    [[nodiscard]] std::list<std::unique_ptr<type::type_ast_node>> &get_exceptions_thrown();
};

}
