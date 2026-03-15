#pragma once

#include "defenition/i_pretty_string_convertable.h"
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


class type_ast_node : public impl::ast_node, public impl::i_descriptor_emitter,
    public definition::i_pretty_string_convertable
{
    int array_dimensions;

protected:
    [[nodiscard]] virtual output::ir::instruction_type _to_instruction_type() const = 0;

public:
    explicit type_ast_node(blasphemy::code_position code_position);
  /**
   * Generates a type node from the provided descriptor
   * @param descriptor
   * @param pos The position where the type descriptor is starting at
   * @param code_position
   */
    static std::unique_ptr<type_ast_node> from_descriptor(const std::string &descriptor, size_t &pos,
            blasphemy::code_position code_position);

    [[nodiscard]] int get_array_dimensions() const;
    void set_array_dimensions(int array_dimensions);

    [[nodiscard]] virtual std::unique_ptr<type_ast_node> clone() const = 0;

    [[nodiscard]] output::ir::instruction_type to_instruction_type() const;
};

}
