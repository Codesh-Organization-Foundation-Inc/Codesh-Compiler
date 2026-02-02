#pragma once

#include "parser/ast/impl/ast_node.h"
#include "parser/ast/impl/i_descriptor_emitter.h"

#include <memory>

namespace codesh::output::ir
{
enum class instruction_type;
}
namespace codesh::ast::type
{

namespace type_decl
{
class attributes_ast_node;
}


class type_ast_node : public impl::ast_node, public impl::i_descriptor_emitter
{
    int array_dimensions;

public:
    explicit type_ast_node(blasphemy::code_position code_position);

    [[nodiscard]] int get_array_dimensions() const;
    void set_array_dimensions(int array_dimensions);

    [[nodiscard]] virtual std::unique_ptr<type_ast_node> clone() const = 0;
    [[nodiscard]] virtual std::string to_pretty_string() const = 0;

    [[nodiscard]] virtual output::ir::instruction_type to_instruction_type() const = 0;
};

}
