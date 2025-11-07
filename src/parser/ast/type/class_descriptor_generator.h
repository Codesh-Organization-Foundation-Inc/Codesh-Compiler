#pragma once
#include "../impl/i_descriptor_emitter.h"
#include "../type_declaration/type_declaration_ast_node.h"

// TODO: Move to utils directory

namespace codesh::util
{

class class_descriptor_generator : ast::impl::i_descriptor_emitter
{
    ast::type_decl::type_declaration_ast_node *type_declaration;
    //TODO: Add package from symbol table

public:
    explicit class_descriptor_generator(ast::type_decl::type_declaration_ast_node *type_declaration);

  [[nodiscard]] std::string generate_descriptor() const override;
};

}
