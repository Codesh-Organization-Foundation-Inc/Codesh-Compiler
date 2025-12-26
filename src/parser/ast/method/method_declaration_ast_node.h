#pragma once

#include "../../../output/jvm_target/class_file_builder.h"
#include "../impl/ast_node.h"
#include "../impl/i_constant_pool_emitter.h"
#include "../impl/i_descriptor_emitter.h"
#include "../impl/i_resolvable.h"
#include "../impl/ir_emitting_ast_node.h"
#include "../local_variable_declaration_ast_node.h"
#include "../type/type_ast_node.h"
#include "../type_declaration/attributes_ast_node.h"

#include <list>
#include <memory>
#include <string>

namespace codesh::semantic_analyzer
{
class method_symbol;
}

namespace codesh::ast::method
{

class method_declaration_ast_node : public impl::ast_node, public impl::i_descriptor_emitter,
        public impl::i_resolvable<semantic_analyzer::method_symbol>, public impl::i_constant_pool_emitter
{
    const definition::fully_qualified_class_name name;
    std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> resolved_symbol;

    std::unique_ptr<type_decl::attributes_ast_node> attributes;

    std::unique_ptr<type::type_ast_node> return_type;
    std::vector<std::unique_ptr<local_variable_declaration_ast_node>> parameters;
    std::vector<std::reference_wrapper<type::type_ast_node>> parameter_types;

    // "throws" declaration
    std::list<std::unique_ptr<type::type_ast_node>> exceptions_thrown;

    std::list<std::unique_ptr<impl::ir_emitting_ast_node>> body;
    std::list<std::unique_ptr<local_variable_declaration_ast_node>> local_variables;

protected:
    [[nodiscard]] const std::optional<std::reference_wrapper<semantic_analyzer::method_symbol>> &_get_resolved()
        const override;

public:
    explicit method_declaration_ast_node(definition::fully_qualified_class_name name);

    using i_descriptor_emitter::generate_descriptor;
    [[nodiscard]] std::string generate_descriptor(bool resolved) const override;

    [[nodiscard]] std::string generate_parameters_descriptor(bool resolved = true) const;


    void set_resolved(semantic_analyzer::method_symbol &symbol) override;
    [[nodiscard]] const definition::fully_qualified_class_name &get_unresolved_name() const override;


    [[nodiscard]] type_decl::attributes_ast_node *get_attributes() const;
    void set_attributes(std::unique_ptr<type_decl::attributes_ast_node> attributes);

    [[nodiscard]] type::type_ast_node *get_return_type() const;
    void set_return_type(std::unique_ptr<type::type_ast_node> return_type);


    [[nodiscard]] std::list<std::unique_ptr<impl::ir_emitting_ast_node>> &get_body();
    [[nodiscard]] const std::list<std::unique_ptr<impl::ir_emitting_ast_node>> &get_body() const;

    [[nodiscard]] std::list<std::unique_ptr<local_variable_declaration_ast_node>> &get_local_variables();
    [[nodiscard]] const std::list<std::unique_ptr<local_variable_declaration_ast_node>> &get_local_variables() const;

    [[nodiscard]] const std::vector<std::unique_ptr<local_variable_declaration_ast_node>> &get_parameters() const;
    void add_parameter(std::unique_ptr<local_variable_declaration_ast_node> parameter);

    [[nodiscard]] const std::list<std::unique_ptr<type::type_ast_node>> &get_exceptions_thrown() const;
    [[nodiscard]] std::list<std::unique_ptr<type::type_ast_node>> &get_exceptions_thrown();

    void emit_constants(const compilation_unit_ast_node &root_node,
                        output::jvm_target::constant_pool &constant_pool) const override;
};

}
